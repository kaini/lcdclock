#include "../datetime.hpp"
#include <iosfwd>

namespace datetime {
std::ostream& operator<<(std::ostream& out, const datetime& dt);
}
