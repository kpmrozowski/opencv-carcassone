#include <Carcassonne/Game/Board.h>

namespace carcassonne::game {

int Board::min_x() const noexcept {
   return m_min_x;
}

int Board::min_y() const noexcept {
   return m_min_y;
}

int Board::max_x() const noexcept {
   return m_max_x;
}

int Board::max_y() const noexcept {
   return m_max_y;
}

TilePlacement Board::tile_at(int x, int y) const noexcept {
   if (x < 0 || y < 0 || x >= g_board_width || y >= g_board_height)
      return TilePlacement{.type = 0};
   return m_board.get(x, y);
}

void Board::set_tile(int x, int y, TileType t, mb::u8 rotation) noexcept {
   if (x < 0 || y < 0 || x >= g_board_width || y >= g_board_height)
      return;

   if (x < m_min_x) {
      m_min_x = x;
   } else if (x >= m_max_x) {
      m_max_x = x+1;
   }
   if (y < m_min_y) {
      m_min_y = y;
   } else if (y >= m_max_y) {
      m_max_y = y+1;
   }

   m_board.set(x, y, TilePlacement{.type = t, .rotation = rotation});
}

TilePlacement Board::tile_at(TilePosition pos) const noexcept {
   return tile_at(pos.x, pos.y);
}

}// namespace carcassonne::game