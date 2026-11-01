/*
 * F4EGM APRS QSO Guard RC2, OpenGD77 R20260131, MDUV380 target.
 * Independent of the F4EGM Dual Watch channel modification.
 * Inherit OpenGD77 modified BSD-3 conditions, including noncommercial use.
 * No AMBE/DMR proprietary code is included.
 */
#ifndef F4EGM_APRS_QSO_GUARD_H
#define F4EGM_APRS_QSO_GUARD_H
#include <stdbool.h>
#include <stdint.h>

/*
 * A 350 ms RX-priority probe in F4EGM DW8.1 can temporarily mute DW:T.
 * Requiring 500 ms of uninterrupted silence prevents APRS interpreting
 * this single-receiver probe as a QSO gap. This is not a 30 s QSO timeout.
 */
#define F4EGM_APRS_QSO_HANG_MS 500U
/* Retry only an already-deferred automatic beacon (not new Smart timing). */
#define F4EGM_APRS_QSO_RETRY_MS 100U

typedef struct
{
    uint32_t lastActivityMs;
    bool hadActivity;
} f4egmAprsQsoGuard_t;

/*
 * The caller supplies the current RF audio/squelch and PTT state.
 * Every APRS tick refreshes this state; wrapping uint32_t subtraction
 * handles the 49.7-day millisecond-counter rollover.
 * No time penalty for a clean startup without RX/PTT activity.
 */
static inline bool f4egmAprsQsoGuardBusy(f4egmAprsQsoGuard_t *guard,
                                         uint32_t nowMs,
                                         bool receivingVoiceOrPtt)
{
    if (receivingVoiceOrPtt)
    {
        guard->lastActivityMs = nowMs;
        guard->hadActivity = true;
        return true;
    }
    if (!guard->hadActivity)
    {
        return false;
    }
    if ((uint32_t)(nowMs - guard->lastActivityMs) < F4EGM_APRS_QSO_HANG_MS)
    {
        return true;
    }
    guard->hadActivity = false;
    return false;
}
#endif
