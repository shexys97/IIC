//Prototypes
void PWM_vfnInitDriver(void);
void PWM_vfnSetCV(uint_8 *bpCV, uint_8 *bpOp);

//Defines
#define PWM_CENTER_ALIGN_CONFIG 0b00101000
#define PWM_MOD 4000
#define PWM_CNV_OFFSET  (PWM_MOD/2)
#define PWM_SC_CONFIG 46
#define PWM_SPEED 2
