#ifndef DATABASE_UTILS_H
#define DATABASE_UTILS_H

namespace Database {
#define loopRange(i, begin, end) for(int i=(begin); i<(end); i++)
#define loop(i, end) loopRange(i, 0, (end))
#define loopn(i, count) loopRange(i, 1, (count)+1)
}
#endif
