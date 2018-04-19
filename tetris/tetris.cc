#include <vector>
#include <string>
#include <ostream>
#include <fstream>
#include <set>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <unistd.h>
#include <ncurses.h>

class Tetris {
  public:
    Tetris(size_t n_row = 0, size_t n_col = 0) : n_row_(n_row), n_col_(n_col), 
    win_(0), score_(0), current_(0), next_(0) { }
    ~Tetris() {
      if (win_) { delwin(win_); win_ = 0; }
      if (current_) { delete current_; current_ = 0; }
      if (next_) { delete next_; next_ = 0; }
      endwin();
    }
    bool init();
    bool run();
    void gameOver();
  private:
    enum { CC = 'H' };
    enum BlockStatus { kBlockValid = 0, kLrBorderReached, kBottomReached, kBlockReached, kLrBorderOverlapped, kBottomOverlapped, kBlockOverlapped, kGameOver };
    class Block {
      public:
        static Block * getRandomBlock() { return new Block(rand() % n_block_); }
        BlockStatus show(WINDOW * win, size_t row, size_t col, std::vector<std::vector<bool> > * bm = NULL, size_t * score = NULL, chtype c = CC);
        Block(Block * blk) { for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) bitmap_[i][j] = blk->bitmap_[i][j]; }
        void rotateClockwise();
      private:
        Block(size_t idx = 0);
      private:
        int bitmap_[4][4];
        const static int n_block_ = 7;
    };
    void showNext(chtype c = CC) { next_->show(stdscr, 4, n_col_ + 2 + 1 + 1, NULL, NULL, c); }
    void updateScore() const { mvprintw((n_row_ >> 1) + 2, n_col_ + 2 + 1, "%d", score_); refresh(); }
    void switchToNext() {
      updateScore();
      showNext(' ');                  // Make next vanish.
      delete current_;
      current_ = next_;
      next_ = Block::getRandomBlock();
      showNext();
    }
  private:
    size_t n_row_;
    size_t n_col_;
    WINDOW * win_;
    std::vector<std::vector<bool> > board_;
    size_t score_;
    Block * current_;
    Block * next_;
    static const size_t n_min_row_ = 7;             // For score and border display.
    static const size_t n_margin_col_ = 9 + 2 + 1;  // For score, border display and a space between them.
};

bool Tetris::init() {
  initscr();
  //raw();
  cbreak();
  refresh();
  noecho();
  keypad(stdscr, TRUE);
  start_color();
  curs_set(0);
  if (n_row_ == 0 || n_col_ == 0) {
    n_row_ = LINES - 2;
    n_col_ = COLS - n_margin_col_;
  } else if (n_row_ + 2 > LINES || n_col_ + n_margin_col_ > COLS || 
      LINES < n_min_row_ || COLS < n_margin_col_) {
    endwin();
    printf("Your game is too big or your terminal is too small.\n");
    return false;
  }
  // Set color and get a border.
  init_pair(1, COLOR_RED, COLOR_BLACK);
  wbkgd(stdscr, COLOR_PAIR(1));
  refresh();
  win_ = newwin(n_row_ + 2, n_col_ + 2, 0, 0);
  init_pair(2, COLOR_WHITE, COLOR_BLACK);
  wbkgd(win_, COLOR_PAIR(2));
  //box(win_, 'H', 'Z');
  wborder(win_, 'H', 'H', 'Z', 'Z', 'H', 'H', 'H', 'H');
  wrefresh(win_);
  delwin(win_);
  // Get play window.
  win_ = newwin(n_row_, n_col_, 1, 1);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  wbkgd(win_, COLOR_PAIR(3));
  board_ = std::vector<std::vector<bool> >(n_row_, std::vector<bool>(n_col_));
  // Next and score on stdscr.
  mvprintw(0, n_col_ + 2 + 1, "Next: ");
  mvprintw((n_row_ >> 1) + 1, n_col_ + 2 + 1, "Score:");
  refresh();
  srand(time(0));
  current_ = Block::getRandomBlock();
  next_ = Block::getRandomBlock();
  showNext();
  updateScore();
  return true;
}

Tetris::Block::Block(size_t idx) {
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      bitmap_[i][j] = 0;
  switch (idx) {
    case 0: bitmap_[0][0] = bitmap_[0][1] = bitmap_[0][2] = bitmap_[1][1] = 1; break;
    case 1: bitmap_[0][1] = bitmap_[0][2] = bitmap_[1][0] = bitmap_[1][1] = 1; break;
    case 2: bitmap_[0][0] = bitmap_[0][1] = bitmap_[1][1] = bitmap_[1][2] = 1;break;
    case 3: bitmap_[0][1] = bitmap_[1][1] = bitmap_[2][1] = bitmap_[2][0] = 1;break;
    case 4: bitmap_[0][0] = bitmap_[1][0] = bitmap_[2][0] = bitmap_[2][1] = 1;break;
    case 5: bitmap_[0][0] = bitmap_[1][0] = bitmap_[2][0] = bitmap_[3][0] = 1;break;
    case 6: bitmap_[0][0] = bitmap_[0][1] = bitmap_[1][0] = bitmap_[1][1] = 1;break;
    default: mvprintw(1, 1, "Internal block error.\n"); refresh(); break;
  }
}

void Tetris::Block::rotateClockwise() {
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4 - i; ++j)
      std::swap(bitmap_[i][j], bitmap_[3-j][3-i]);
  for (int i = 0; i < 4 >> 1; ++i)
    for (int j = 0; j < 4; ++j)
      std::swap(bitmap_[i][j], bitmap_[3-i][j]);
}

// row and col are relative to win's start_y and start_x.
Tetris::BlockStatus Tetris::Block::show(WINDOW * win, size_t row, size_t col, std::vector<std::vector<bool> > * bm, size_t * score, chtype c) {
  // Get coordinates of squres in Block.
  std::vector<std::pair<int, int> > points;	    // Relative to win's starts.
  int rr = -1, cc = -1;       		              // Left bottom one.
  for (int ri = 3; ri >= 0; --ri)
    for (int ci = 0; ci < 4; ++ci)
      if (bitmap_[ri][ci]) {
        if (rr == -1 && cc == -1) { rr = ri; cc = ci; }
        points.push_back(std::make_pair(ri - rr + int(row), ci - cc + int(col)));
      }
  // Get win size and check.
  int beg_r = 0, beg_c = 0, max_r = 0, max_c = 0;
  getbegyx(win, beg_r, beg_c);
  getmaxyx(win, max_r, max_c);
  int n_row = max_r - beg_r + 1, n_col = max_c - beg_c + 1;
  bool bottom_reached = false, block_reached = false;
  for (const auto & p : points) {
    if (p.first >= n_row) return kBottomOverlapped;
    else if (p.first == n_row - 1) bottom_reached = true;
    if (p.second < 0 || p.second >= n_col) return kLrBorderOverlapped;
  }
  if (bm)
    for (const auto & p : points)
      if (p.first >= 0 && p.second >= 0 && 
          p.first < n_row && p.second < n_col) {
        if ((*bm)[p.first][p.second]) {
          if (p.first) return kBlockOverlapped;
          else return kGameOver;
        }
        if (p.first < n_row - 1 && (*bm)[p.first + 1][p.second])
          block_reached = true;
      }
  // Display and mark.
  for (const auto & p : points)
    if (p.first >= 0) {
      mvwaddch(win, p.first, p.second, c | A_BOLD);
      if (score) (*bm)[p.first][p.second] = true;
    }
  wrefresh(win);
  // Collapse and calculate score.
  if (score) {
    int empty_idx = -1;
    std::set<int, std::greater<int> > coll_idx;
    auto & bd = *bm;
    for (int i = n_row - 1; i >= 0; --i) {
      bool full = true, empty = true;
      for (auto p : bd[i]) {
        if (!p) full = false;
        else empty = false;
        if (!full && !empty) break;
      }
      if (empty) { empty_idx = i; break; }
      if (full) {
        coll_idx.insert(i);
        if (coll_idx.size() >= 4) break;
      }
    }
    if (coll_idx.size()) {
      std::vector<bool> empty_row(n_col, false);
      int j = *(coll_idx.begin());
      int i = j - 1;
      for (; i > empty_idx; --i)
        if (coll_idx.find(i) == coll_idx.end())
          bd[j--] = bd[i];
      for (; j > empty_idx; --j)
        bd[j] = empty_row;
      // Refresh.
      for (size_t ri = 0; ri < n_row; ++ri)
        for (size_t ci = 0; ci < n_col; ++ci) {
          if (bd[ri][ci]) mvwaddch(win, ri, ci, c | A_BOLD);
          else mvwaddch(win, ri, ci, ' ');
        }
      wrefresh(win);
      *score += (1 << (coll_idx.size() - 1)) * n_col;
    }
  }
  return bottom_reached ? kBottomReached : block_reached ? kBlockReached : kBlockValid;
}

bool Tetris::run() {
  // Init head position.
  size_t hr0 = 0, hc0 = n_col_ >> 1;
  size_t hr = hr0, hc = hc0;
  while (1) {
    // Show current Block at first.
    BlockStatus status = current_->show(win_, hr, hc, &board_);
    if (status == kGameOver) return false;
    // Wait for down/left/right/up.
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 300000;
    int n_set = select(1, &rfds, 0, 0, &tv);
    if (n_set == 0) {
      if (status == kBottomReached || status == kBlockReached) {
        current_->show(win_, hr, hc, &board_, &score_);
        switchToNext();
        hr = hr0; hc = hc0;
      } else if (status == kBlockValid) {
        // Displayed correctly.
        current_->show(win_, hr, hc, NULL, NULL, ' ');   // Clear current block display.
        ++ hr;
      } else {
        wprintw(win_, "%d", status);
        wrefresh(win_);
        assert(0);
      }
    } else if (n_set == 1) {
      int ch = getch();
      if (ch == KEY_LEFT || ch == KEY_RIGHT) {
        if ((ch == KEY_LEFT && hc > 0) || (ch == KEY_RIGHT && hc < n_col_)) {
          current_->show(win_, hr, hc, NULL, NULL, ' ');
          BlockStatus bs = current_->show(win_, hr, ch == KEY_LEFT ? hc - 1 : hc + 1, &board_);
          if (bs != kLrBorderOverlapped && bs != kBlockOverlapped)
            ch == KEY_LEFT ? -- hc : ++ hc;
        }
      } else if (ch == KEY_UP) {
        current_->show(win_, hr, hc, NULL, NULL, ' ');
        Block * blk = new Block(current_);
        blk->rotateClockwise();
        BlockStatus bs = blk->show(win_, hr, hc, &board_);
        if (bs != kLrBorderOverlapped && bs != kBottomOverlapped && bs != kBlockOverlapped) {
          delete current_;
          current_ = blk;
        } else {
          delete blk;
        }
      } else if (ch == KEY_DOWN) {
        if (status == kBottomReached || status == kBlockReached) {
          current_->show(win_, hr, hc, &board_, &score_);
          switchToNext();
          hr = hr0; hc = hc0;
        } else {
          current_->show(win_, hr, hc, NULL, NULL, ' ');
          ++ hr;
        }
      } else if (ch == ' ') {
        while (getch() != ' ');
      }
    } else {
      printw("Internal timer error.\n");
      return false;
    }
  }
}

void Tetris::gameOver() {
  int c = (n_col_ >> 1) - 4;
  WINDOW * go_win = newwin(2, 10, n_row_ >> 1, c > 0 ? c : 0);
  wbkgd(go_win, COLOR_PAIR(1));
  wprintw(go_win, "GAME OVER!");
  for (int i = 5; i > 0; --i) {
    mvwprintw(go_win, 1, 4, "%d", i);
    wrefresh(go_win);
    sleep(1);
  }
}

int main(int argc, const char * argv[]) {
  size_t n_row = 0, n_col = 0;
  if (argc >= 3) {
    n_row = std::stoi(argv[1]);
    n_col = std::stoi(argv[2]);
  }
  if (n_row == 0 || n_col == 0) {
    printf("You didn't give me numbers of row or column of the game, I'll show the largest to you.\nHit Enter to continue.\n");
    getchar();
  }

  Tetris tetris(n_row, n_col);
  if (!tetris.init()) {
    printf("Tetris initialization failed.\n");
    return -1;
  }
  if (!tetris.run())
    tetris.gameOver();

  return 0;
}
