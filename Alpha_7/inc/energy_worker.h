#ifndef _BC_ENERGY_WORKER_H_
#define _BC_ENERGY_WORKER_H_

#define BACKLIGHT_PIN 2
#define PWM_FREQUENCY 5000
#define PWM_CHANNEL 0
#define PWM_RESOLUTION 8

class energyWorker {

private:
	uint16_t m_backlIntencity;
	uint16_t m_backlFadeSec;

public:

	energyWorker() {
		
		ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
		ledcAttachPin(BACKLIGHT_PIN, PWM_CHANNEL);
	}
	
	void setBacklightIntencity(uint16_t backlIntencity) {
		m_backlIntencity = backlIntencity;
		ledcWrite(PWM_CHANNEL, m_backlIntencity);
	}

	void setBackliteDuration(uint16_t backlFadeSec) {
		m_backlFadeSec = backlFadeSec;
		//TODO: убрать заглушку...
	}
};

#endif /* _BC_ENERGY_WORKER_H_ */