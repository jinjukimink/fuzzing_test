#include <inttypes.h>
#include <sndfile.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fuzzer/FuzzedDataProvider.h>

#include "sndfile_fuzz_header.h"

extern "C" {
#include "sndfile.h"
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // 입력 데이터가 너무 작으면 테스트하지 않음
    if (size < 4) {
        return 0;
    }

    // FuzzedDataProvider로 데이터 처리
    FuzzedDataProvider stream(data, size);

    // libsndfile 관련 구조체 초기화
    SF_INFO sndfile_info;
    VIO_DATA vio_data;
    SF_VIRTUAL_IO vio;
    SNDFILE *sndfile = NULL;

    // sf_init_file로 파일 초기화
    int err = sf_init_file(data, size, &sndfile, &vio_data, &vio, &sndfile_info);
    if (err) {
        if (sndfile != NULL) {
            sf_close(sndfile);
        }
        return 0;
    }

    // 무작위 명령 선택
    int cmd = stream.ConsumeIntegral<int>();

    // 남은 데이터를 명령 데이터로 사용
    std::vector<uint8_t> cmd_data = stream.ConsumeRemainingBytes<uint8_t>();
    int data_size = cmd_data.size();

    // sf_command 호출
    sf_command(sndfile, cmd, cmd_data.data(), data_size);

    // 리소스 정리
    if (sndfile != NULL) {
        sf_close(sndfile);
    }

    return 0;
}
