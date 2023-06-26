#ifndef SHA1_H
#define SHA1_H

/*
   SHA-1 in C
   By Steve Reid <steve@edmweb.com>
   100% Public Domain
 */
 // Description: Functions to calculate SHA1
 // Class implementation of the following repository: https://github.com/clibs/sha1

#include "stdint.h"
#include <string>

#if defined(__cplusplus)
extern "C" {
#endif

    namespace ce {
        typedef struct
        {
            uint32_t state[5];
            uint32_t count[2];
            unsigned char buffer[64];
        } SHA1_CTX;

        class ceSHA1 {
        private:
            SHA1_CTX sha;
            //-----------------------------------------------------------------------------
            void SHA1Transform(
                uint32_t state[5],
                const unsigned char buffer[64]
            );

            void SHA1Init(
                SHA1_CTX* context
            );

            void SHA1Update(
                SHA1_CTX* context,
                const unsigned char* data,
                uint32_t len
            );

            void SHA1Final(
                unsigned char digest[20],
                SHA1_CTX* context
            );
            void SHA1(
                char* hash_out,
                const char* str,
                uint32_t len);
        public:
            void Init();
            void Update(std::string str);
            std::string Final();
        };
    } // namespace ce 
#if defined(__cplusplus)
}
#endif

#endif /* SHA1_H */
