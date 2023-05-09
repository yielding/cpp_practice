#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory>
#include <cstring>
#include <cstdarg>
#include <vector>
#include <unordered_map>
#include <string>
#include <list>
#include <queue>
#include <deque>
#include <unordered_set>

typedef int SOCKET;
const int NO_ERROR = 0;
const int INVALID_SOCKET = -1;
const int WSAECONNRESET = ECONNRESET;
const int WSAEWOULDBLOCK = EAGAIN;
const int SOCKET_ERROR = -1;

using namespace std;
using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using std::queue;
using std::list;
using std::deque;
using std::unordered_map;
using std::string;
using std::unordered_set;

#include "StringUtils.h"
#include "SocketAddress.h"
#include "TCPSocket.h"
#include "SocketUtil.h"