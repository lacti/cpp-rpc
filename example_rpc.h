#ifndef _RPC_GENERATING_

#   define _RPC_SERVER_
#   define _RPC_CLIENT_
#   define _RPC_GEN_TARGET_ "example_rpc.h"
#   include "rpc_gen.h"

#else

RPC_BEGIN(example)
    RPC_METHOD2(int, add, int, a, int, b)
    RPC_METHOD2(double, add, double, a, double, b)
    RPC_METHOD3(int, add, int, a, int, b, int, c)
RPC_END(example)

#endif