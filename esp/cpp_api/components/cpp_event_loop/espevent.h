/*
 * espevent.h
 *
 *  Created on: 9 Mar 2020
 *      Author: a1
 */

#ifndef COMPONENTS_CPP_EVENT_LOOP_ESPEVENT_H_
#define COMPONENTS_CPP_EVENT_LOOP_ESPEVENT_H_

#include <esp_event.h>

#include <memory>

namespace amosoft {

class EspSystemEventLoop;
class EspEventLoop;

class EspEventBaseImpl;

template <class T, esp_event_base_t EVENT_BASE/*, int32_t EVENT_ID*/>
class EspEventBase { // TODO: add ESP_BASE constant to the template arguments and EVENT typename
public:
	EspEventBase(EspEventBase&&) = default;
	EspEventBase& operator=(EspEventBase&&) = default;

	friend EspSystemEventLoop;
	friend EspEventLoop;

	constexpr esp_event_base_t base() noexcept { return EVENT_BASE; }
	/*constexpr esp_event_base_t id() noexcept { return EVENT_ID; }*/

private:
	EspEventBase(EspEventBaseImpl* impl):
		m_impl(impl){
	}

	EspEventBaseImpl* releaseImpl() {
		return m_impl.release();
	}

private:
	std::unique_ptr<EspEventBaseImpl> m_impl;
};

class EspCustomEvent : public EspEventBase<EspCustomEvent, WIFI_EVENT> { // TODO: add ESP_EVENT id to template ars
public:
	constexpr static esp_event_base_t event_base = "rrr";
	constexpr static int32_t event_id = 1;

	void initDataImpl(void* data) {

	}

private:
	EspCustomEvent(void* data) :
		EspEventBase<EspCustomEvent>(data) {
	}
};

} /* namespace amosoft */

#endif /* COMPONENTS_CPP_EVENT_LOOP_ESPEVENT_H_ */
