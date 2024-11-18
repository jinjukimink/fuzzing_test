#include <inttypes.h>
#include <sndfile.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <vector>
#include "fuzzer/FuzzedDataProvider.h"
#include "sndfile_fuzz_header.h"

extern "C" {
#include "sndfile.h"
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size <= 4)
        return 0;

    /*데이터를 벡터로 처리 후 배열 포인터로 변환*/
    FuzzedDataProvider fuzzed_data_provider(data, size);
    size_t data_size = fuzzed_data_provider.ConsumeIntegralInRange<size_t>(0, size - 1);
    std::vector<uint8_t> sndfile_data_vec = fuzzed_data_provider.ConsumeBytes<uint8_t>(data_size);
    const uint8_t *sndfile_data = sndfile_data_vec.data();

    SF_INFO sndfile_info;
    VIO_DATA vio_data;
    SF_VIRTUAL_IO vio;
    SNDFILE *sndfile = NULL;

    int cmd;
    int data_size_int;
    int err = sf_init_file(sndfile_data, data_size, &sndfile, &vio_data, &vio, &sndfile_info);
    if (err) {
        if (sndfile != NULL)
            sf_close(sndfile);
        return 0;
    }

    cmd = fuzzed_data_provider.ConsumeIntegral<int>();
    data_size_int = fuzzed_data_provider.ConsumeIntegral<int>();

    sf_command(sndfile, cmd, (void *)sndfile_data, data_size_int);

    if (sndfile != NULL)
        sf_close(sndfile);

    return 0;
}
