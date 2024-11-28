// FileTransferCheck.c

#include "FileTransferCheck.h"
#include "led.h"
#include "buzzer.h"
#include "main.h"

#define FILE_TRANSFER_TIMEOUT 1000  // 전송 완료 감지 타임아웃 (1초)
#ifdef DEBUG_USE_USB_FILE_TRANSFER_CHECK
static uint32_t last_write_time = 0;  // 마지막 파일 전송 시간
static uint8_t transfer_complete_flag = 0; // 전송 완료 플래그
static uint8_t transfer_active_flag = 0;   // 전송 활성화 플래그
#endif

// LED 상태 초기화 함수
void FileTransferCheck_Init(void)
{
    // 초기 상태로 LED 설정
}

// 파일 전송 중 상태 업데이트 함수
void FileTransferCheck_UpdateOnWrite(void)
{
#ifdef DEBUG_USE_USB_FILE_TRANSFER_CHECK
  // 파일 전송 중 LED1을 토글
  LED_SetState(TARGET_LED_STAT_PROGRAMMING);

  // 마지막 파일 전송 시간을 현재 시간으로 업데이트
  last_write_time = HAL_GetTick();

  // 전송 활성화 플래그 설정 (파일 전송 시작)
  transfer_active_flag = 1;

  // 전송 완료 플래그 리셋
  transfer_complete_flag = 0;
#endif
}

// 파일 전송 완료 상태 감지 함수
void FileTransferCheck_CheckCompletion(void)
{
#ifdef DEBUG_USE_USB_FILE_TRANSFER_CHECK
  // 전송 활성화 플래그가 설정된 경우에만 완료 상태 감지
  if (transfer_active_flag && ((HAL_GetTick() - last_write_time) > FILE_TRANSFER_TIMEOUT))
  {
      // 파일 전송 완료: LED1을 OFF하고, LED2를 ON 상태로 설정
      LED_SetState(TARGET_LED_STAT_COMPLETE);

      // 전송 완료 플래그 설정
      transfer_complete_flag = 1;

      // 전송 활성화 플래그 리셋 (다음 전송을 대기)
      transfer_active_flag = 0;
  }
#endif
}

// 파일 전송 완료 플래그 확인 함수
uint8_t FileTransferCheck_IsTransferComplete(void)
{
#ifdef DEBUG_USE_USB_FILE_TRANSFER_CHECK
    return transfer_complete_flag;
#else
    return 0;
#endif
}

// 파일 전송 완료 플래그 리셋 함수
void FileTransferCheck_ResetTransferCompleteFlag(void)
{
#ifdef DEBUG_USE_USB_FILE_TRANSFER_CHECK
    transfer_complete_flag = 0;
#endif
}

// 파일 전송 완료 상태를 처리하는 함수
void FileTransferCheck(void)
{
#ifdef DEBUG_USE_USB_FILE_TRANSFER_CHECK
    if (FileTransferCheck_IsTransferComplete())
    {
    	Buzzer_SetState(BUZZER_PROG_COMPLETE);               // 버저 완료 사운드 재생
      FileTransferCheck_ResetTransferCompleteFlag(); // 플래그 리셋
    }
#endif
}
