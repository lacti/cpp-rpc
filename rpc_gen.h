#include "rpc.h"
#define _RPC_GENERATING_ 1
#   define _RPC_GEN_TYPE_
#     include "rpc_gen_spec.h"
#   undef _RPC_GEN_TYPE_

#   ifdef _RPC_SERVER_
#     define _RPC_GEN_SERVER_
#        include "rpc_gen_spec.h"
#       undef _RPC_GEN_SERVER_
#   endif

#   ifdef _RPC_CLIENT_
#      define _RPC_GEN_CLIENT_
#       include "rpc_gen_spec.h"
#     undef _RPC_GEN_CLIENT_
#   endif
#undef _RPC_GENERATING_