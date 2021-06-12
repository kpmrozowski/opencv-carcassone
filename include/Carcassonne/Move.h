#ifndef MSI_CARCASSONNE_MOVEITERATOR_H
#define MSI_CARCASSONNE_MOVEITERATOR_H
#include "IBoard.h"
#include "Player.h"
#include <memory>
#include <mb/result.h>

namespace carcassonne {

struct TileMove {
   int x, y;
   mb::u8 rotation;

   constexpr bool operator==(const TileMove &other) const {
      return x == other.x && y == other.y && rotation == other.rotation;
   }

   constexpr bool operator!=(const TileMove &other) const {
      return x != other.x || y != other.y || rotation != other.rotation;
   }
};

struct FullMove {
   int x{}, y{};
   mb::u8 rotation{};
   bool ignored_figure = true;
   Direction direction{};
};

class MoveIter {
   TileMove m_move;
   TileType m_tile_type;
   std::reference_wrapper<const IBoard> m_board;

 public:
   typedef MoveIter self_type;
   typedef TileMove value_type;
   typedef TileMove &reference;
   typedef const TileMove &const_reference;
   typedef TileMove *pointer;
   typedef const TileMove *const_pointer;
   typedef std::bidirectional_iterator_tag iterator_category;
   typedef mb::size difference_type;

   constexpr explicit MoveIter(const IBoard &board, TileType tt) : m_move{
                                                                           .x = board.min_x() - 1,
                                                                           .y = board.min_y() - 1,
                                                                           .rotation = 0,
                                                                   },
                                                                   m_tile_type(tt), m_board(board) {
      seek_next();
   }

   constexpr explicit MoveIter(const IBoard &board, TileType tt, int x, int y, mb::u8 rotation) : m_move{
                                                                                                          .x = x,
                                                                                                          .y = y,
                                                                                                          .rotation = rotation,
                                                                                                  },
                                                                                                  m_tile_type(tt), m_board(board) {
   }

   constexpr self_type operator++() {
      self_type tmp_it(*this);
      progress();
      seek_next();
      return tmp_it;
   }

   constexpr self_type operator++(int) {
      progress();
      seek_next();
      return *this;
   }

   constexpr self_type operator--() {
      self_type tmp_it(*this);
      regress();
      seek_prev();
      return tmp_it;
   }

   constexpr self_type operator--(int) {
      regress();
      seek_prev();
      return *this;
   }

   constexpr self_type operator+(difference_type count) const {
      self_type result(*this);
      for (difference_type i = 0; i < count; ++i) {
         result.progress();
         result.seek_next();
      }
      return result;
   }

   constexpr self_type operator-(difference_type count) const {
      self_type result(*this);
      for (difference_type i = 0; i < count; ++i) {
         result.regress();
         result.seek_prev();
      }
      return result;
   }

   constexpr difference_type operator-(const self_type &other) const {
      difference_type count{};
      for (self_type at = other; at != *this; ++at) {
         ++count;
      }
      return count;
   }

   constexpr reference operator*() {
      return m_move;
   }

   constexpr const_pointer operator->() const {
      return &m_move;
   }

   constexpr bool operator==(const self_type &other) const {
      return m_move == other.m_move;
   }

   constexpr bool operator!=(const self_type &other) const {
      return m_move != other.m_move;
   }

 private:
   constexpr bool progress() {
      if (m_move.rotation < 3) {
         ++m_move.rotation;
         return true;
      }
      m_move.rotation = 0;

      if (m_move.x < m_board.get().max_x()) {
         ++m_move.x;
         return true;
      }
      m_move.x = m_board.get().min_x() - 1;

      if (m_move.y < m_board.get().max_y()) {
         ++m_move.y;
         return true;
      }

      m_move.y = m_board.get().max_y() + 1;
      return false;
   }

   constexpr bool regress() {
      if (m_move.rotation > 0) {
         --m_move.rotation;
         return true;
      }
      m_move.rotation = 3;

      if (m_move.x >= m_board.get().min_x()) {
         --m_move.x;
         return true;
      }
      m_move.x = m_board.get().max_x();

      if (m_move.y >= m_board.get().min_y()) {
         --m_move.y;
         return true;
      }

      m_move.y = m_board.get().min_y() - 1;
      return false;
   }

   constexpr void seek_prev() {
      do {
         if (m_board.get().can_place_at(m_move.x, m_move.y, m_tile_type, m_move.rotation))
            return;
      } while (regress());
   }

   constexpr void seek_next() {
      do {
         if (m_board.get().can_place_at(m_move.x, m_move.y, m_tile_type, m_move.rotation))
            return;
      } while (progress());
   }
};

struct MoveRange {
   MoveIter from;
   MoveIter to;

   [[nodiscard]] constexpr MoveIter begin() const {
      return from;
   }

   [[nodiscard]] constexpr MoveIter end() const {
      return to;
   }
};

enum class MovePhase {
   PlaceTile,
   PlaceFigure,
   Done
};

class IGame;

class IMove {
 public:
   [[nodiscard]] virtual Player player() const noexcept = 0;
   [[nodiscard]] virtual TileType tile_type() const noexcept = 0;
   [[nodiscard]] virtual MovePhase phase() const noexcept = 0;
   [[nodiscard]] virtual bool is_free(Direction d) const noexcept = 0;
   [[nodiscard]] virtual TilePosition position() const noexcept = 0;
   [[nodiscard]] virtual std::unique_ptr<IMove> clone(IGame &game) const noexcept = 0;
   virtual mb::result<mb::empty> place_tile_at(int x, int y, mb::u8 rotation) noexcept = 0;
   virtual mb::result<mb::empty> place_tile(TileMove tile_location) noexcept = 0;
   virtual mb::result<mb::empty> place_figure(Direction d) noexcept = 0;
   virtual mb::result<mb::empty> ignore_figure() noexcept = 0;
};

}// namespace carcassonne

#endif//MSI_CARCASSONNE_MOVE_H