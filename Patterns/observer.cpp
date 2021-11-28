#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

/*** Interfaces ***/

class iObserver {

public:
	iObserver(){}

	virtual void update(float temp, float pressure, float humidity) = 0;
	virtual void display() = 0;
	virtual ~iObserver(){}
};

class iSubject {

public:
	iSubject(){}

	virtual void registerObserver(std::shared_ptr<iObserver> observer) = 0;
	virtual void remoteObserver(std::shared_ptr<iObserver> observer) = 0;
	virtual void notifyObserver() = 0;
	virtual ~iSubject(){}
};

/******************/

class WeatherData : public iSubject {

private:
	std::vector<std::shared_ptr<iObserver>> m_observers;
	float m_temp, m_pressure, m_humidity;

	/* Собственный (не виртуальный метод) */
	void measurementsChanged() {

		notifyObserver();
	}

public:

	/* Реализация интефейса iSubject */
	void registerObserver(std::shared_ptr<iObserver> observer) override {

		m_observers.push_back(observer);
	}
	void remoteObserver(std::shared_ptr<iObserver> observer) override {

		m_observers.erase(std::find(m_observers.begin(), m_observers.end(), observer));
	}
	void notifyObserver() override {

		for(const auto& observer : m_observers) {

			observer->update(m_temp, m_pressure, m_humidity);
		}
	}

	/* Сеттер имитирующий получение реальных погодных данных 
	и оповещение наблюдателей об изменениях */
	void setMeasurements(float temp, float pressure, float humidity) {

		m_temp = temp;
		m_pressure = pressure;
		m_humidity = humidity;

		measurementsChanged();
	}
};

class CurrentConditionsDisplay : public iObserver {

private:
	float m_temp, m_pressure, m_humidity;

public:

	CurrentConditionsDisplay(){}

	void update(float temp, float pressure, float humidity) override {

		m_temp = temp;
		m_pressure = pressure;
		m_humidity = humidity;
	}
	void display() override {

		std::cout << "Метеокондиции: T = " << m_temp << " P = " 
		<< m_pressure << " H = " << m_humidity << std::endl;
	}
};


int main() {

	WeatherData wd;
	std::shared_ptr<iObserver> cd(new CurrentConditionsDisplay);
	wd.registerObserver(cd);

	//Как бы регистрация еще каких-то наблюдателей...
	//std::shared_ptr<iObserver> sd(new StatisticsDisplay);
	//wd.registerObserver(sd);
	//std::shared_ptr<iObserver> fd(new ForecastDisplay);
	//wd.registerObserver(fd);

	/***** Имитация новых данных *****/
	wd.setMeasurements(80, 65, 30.4f);
	cd->display();
	//sd->display();
	//fd->display();
	//и т.д.

	wd.setMeasurements(82, 70, 29.2f);
	cd->display();

	wd.setMeasurements(78, 90, 29.2f);
	cd->display();
	return 0;
}