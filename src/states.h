#ifndef CAR_STATES_H
#define CAR_STATES_H

void state_idle_init();
void state_idle_update_fixed();
void state_idle_update_fast();

void state_drive_init();
void state_drive_update_fixed();
void state_drive_update_fast();

void state_recover_init();
void state_recover_update_fixed();
void state_recover_update_fast();

void state_error_init();
void state_error_update_fixed();
void state_error_update_fast();

#endif
