#ifndef INCLUDE_UTILS_PCPLATFORMMACROS_H_
#define INCLUDE_UTILS_PCPLATFORMMACROS_H_

// generic macros

// namespace  parsecloud{}
#ifdef __cplusplus
#define NS_PC_BEGIN                     namespace parsecloud{
#define NS_PC_END                       }
#define USING_NS_PC                     using namespace parsecloud
#else
#define NS_PC_BEGIN
#define NS_PC_END
#define USING_NS_PC
#endif

#define PC_SAFE_DELETE(p)           do { delete (p);} while(0)
#define AV_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); } } while(0)

#endif  // INCLUDE_UTILS_PCPLATFORMMACROS_H_
