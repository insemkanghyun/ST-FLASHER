#ifndef BUZZER_H_
#define BUZZER_H_

typedef enum {
    BUZZER_BOOT,
		BUZZER_PROG_START,
		BUZZER_PROG_COMPLETE,
		BUZZER_PROG_FAILED,
		BUZZER_IDLE
} BuzzerState;

void Buzzer_SetState(BuzzerState state);

#endif /* BUZZER_H_ */
