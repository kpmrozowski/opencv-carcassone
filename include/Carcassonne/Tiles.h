#ifndef MSI_CARCASSONNE_TILES_H
#define MSI_CARCASSONNE_TILES_H
#include <Carcassonne/Connection.h>
#include <Carcassonne/Contact.h>
#include <algorithm>
#include <limits>

namespace carcassonne {

enum class EdgeType {
   Grass,
   Path,
   Town,
   Monastery
};

struct Tile {
   std::array<EdgeType, 4> edges{};
   std::array<EdgeType, 8> field_edges{};
   Contact contacts = Contact::None;
   Connection connections = Connection::None;
   bool monastery = false;
   bool pennant = false;
   mb::u8 amount = std::numeric_limits<mb::u8>::infinity();

};

constexpr auto g_max_moves = 71;


using TileType = mb::u8;

struct TilePlacement {
   TileType type = 0;
   mb::u8 rotation = 0;
};


}// namespace carcassonne

#endif//MSI_CARCASSONNE_TILES_H
