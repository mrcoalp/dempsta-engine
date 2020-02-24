#pragma once

#define DE_BIND_EVENT_FN(fn)        std::bind(&fn, this, std::placeholders::_1)
#define DE_ASSERT(status, name)     if (status == 1) {\
                                        LOG_ENGINE_INFO("{0} initialized successfully!", name);\
                                    } else {\
                                        LOG_ENGINE_CRITICAL("{0} initialization failed!", name);\
                                        throw;\
                                    }
