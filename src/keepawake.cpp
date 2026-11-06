#include "keepawake.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <windows.h>

#include <vector>

namespace keepawake
{
namespace
{
// Fills buf with this machine's TCP connections for one address family, each
// with its owning process id. Retries when the table grows between the calls.
bool tcpTable(ULONG family, std::vector<unsigned char> &buf)
{
    DWORD size = 0;
    DWORD ret = ERROR_INSUFFICIENT_BUFFER;
    for (int attempt = 0; attempt < 4 && ret == ERROR_INSUFFICIENT_BUFFER; ++attempt) {
        buf.resize(size);
        ret = GetExtendedTcpTable(buf.empty() ? nullptr : buf.data(), &size, FALSE,
                                  family, TCP_TABLE_OWNER_PID_CONNECTIONS, 0);
    }
    return ret == NO_ERROR && !buf.empty();
}

bool isLoopbackV4(DWORD networkOrderAddr)
{
    return (ntohl(networkOrderAddr) >> 24) == 127;
}

bool isLoopbackV6(const UCHAR addr[16])
{
    for (int i = 0; i < 15; ++i) {
        if (addr[i] != 0) return false;
    }
    return addr[15] == 1;
}
} // namespace

bool hasClientConnection()
{
    const DWORD pid = GetCurrentProcessId();
    std::vector<unsigned char> buf;

    if (tcpTable(AF_INET, buf)) {
        const auto *table = reinterpret_cast<const MIB_TCPTABLE_OWNER_PID *>(buf.data());
        for (DWORD i = 0; i < table->dwNumEntries; ++i) {
            const MIB_TCPROW_OWNER_PID &row = table->table[i];
            if (row.dwOwningPid == pid && row.dwState == MIB_TCP_STATE_ESTAB &&
                !isLoopbackV4(row.dwRemoteAddr)) {
                return true;
            }
        }
    }

    if (tcpTable(AF_INET6, buf)) {
        const auto *table = reinterpret_cast<const MIB_TCP6TABLE_OWNER_PID *>(buf.data());
        for (DWORD i = 0; i < table->dwNumEntries; ++i) {
            const MIB_TCP6ROW_OWNER_PID &row = table->table[i];
            if (row.dwOwningPid == pid && row.dwState == MIB_TCP_STATE_ESTAB &&
                !isLoopbackV6(row.ucRemoteAddr)) {
                return true;
            }
        }
    }

    return false;
}

void set(bool on)
{
    SetThreadExecutionState(on ? (ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED)
                               : ES_CONTINUOUS);
}
} // namespace keepawake
