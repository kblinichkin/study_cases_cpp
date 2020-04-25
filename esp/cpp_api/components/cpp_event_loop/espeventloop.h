/*
 * EspEventLoop.h
 *
 *  Created on: 8 Mar 2020
 *      Author: a1
 */

#ifndef COMPONENTS_CPP_EVENT_LOOP_ESPEVENTLOOP_H_
#define COMPONENTS_CPP_EVENT_LOOP_ESPEVENTLOOP_H_

//#include <freertos/FreeRTOS.h>
#include <esp_event.h>
#include <espevent.h>

#include <memory>
#include <map>
#include <functional>
#include <experimental/type_traits>

#include <iostream>

namespace amosoft {

template <class T>
class EspEventLoopBase {
	template<class U>
	using has_constructor =
	    decltype(U(std::declval<void*>()));
public:
	EspEventLoopBase() = default;

	template <
				typename _EventBase,
				typename _Class,
				typename _FuncType
			 >
	void registerHandler(_Class* obj, _FuncType func){
		static_assert(std::is_same<
									void(_Class::*)(const _EventBase&) // TODO: try to find a way to get a type from function parameter
									, _FuncType
								  >::value,
					  "Incorrect callback signature");

		static_assert(std::experimental::is_detected<has_constructor, _EventBase>::value,
				"Incompatible Event class is used");

		static_cast<T*>(this)->template registerHandlerImpl<_EventBase>(obj, func);
	}

	template <
				typename _EventBase,
				typename _FuncType
			 >
	void registerHandler(_FuncType func){
		static_assert(std::is_same<
									void(*)(const _EventBase&)
									, _FuncType
								  >::value,
					  "Incorrect callback signature");

		static_cast<T*>(this)->template registerHandlerImpl<_EventBase>(func);
	}
};

class EspSystemEventLoop final : public EspEventLoopBase<EspSystemEventLoop> {
	template<class _Class, class _Func>
	using CallbackData = std::tuple<_Class*, _Func, esp_event_handler_t>;
	template<class _Class, class _Func>
	using CallbackDataPtr = std::unique_ptr<CallbackData<_Class, _Func>>;
public:
	template <
				typename _EventBase,
				typename _Class,
				typename _Func
			 >
	void registerHandlerImpl(_Class* obj, _Func func) { // TODO: look up for std::enable_if here
		auto callback = [](void* event_handler_arg,
                esp_event_base_t event_base,
                int32_t event_id,
                void* event_data)
		{
			// TODO: probably we do not need event_base/event_id to create an event obj as the class is known in compile time
			const _EventBase event(event_data);
			auto cbData = reinterpret_cast<CallbackData<_Class,_Func>*>(event_handler_arg);

			std::invoke(std::get<1>(*cbData), *std::get<0>(*cbData), event);
		};

		CbData<_Class,_Func>.reset(new CallbackData<_Class,_Func>(obj, func, callback));

		esp_event_handler_register(_EventBase::event_base, _EventBase::event_id, callback, CbData<_Class,_Func>.get());
	}

	template <
				typename _EventBase,
				typename _Func
			 >
	void registerHandlerImpl(_Func func) {

	}


	static EspSystemEventLoop& Instance() {
		static EspSystemEventLoop loop;
		return loop;
	}

private:
	EspSystemEventLoop() {
		esp_event_loop_create_default();
	}

	~EspSystemEventLoop() = default;
	EspSystemEventLoop(const EspSystemEventLoop&) = delete;
	EspSystemEventLoop& operator=(const EspSystemEventLoop&)= delete;

private:
	template<class _Class, class _Func>
	static CallbackDataPtr<_Class,_Func> CbData; // TODO: reset it after unregistering
};

template <class _Class, class _Func>
EspSystemEventLoop::CallbackDataPtr<_Class,_Func> EspSystemEventLoop::CbData;

class EspEventLoop final : public EspEventLoopBase<EspEventLoop> {
	using EvLoopPtr = std::unique_ptr<
			void, // TODO: try to use esp_event_loop_handle_t instead
			decltype(&esp_event_loop_delete)
			>;
public:
	EspEventLoop(const esp_event_loop_args_t *event_loop_args): // TODO: make a wrapper for esp_event_loop_args_t
		m_loop(createLoop(event_loop_args), &esp_event_loop_delete) {
	}

		template <
					typename _EventBase,
					typename _Handler,
					typename _Method
				 >
		void registerHandlerImpl(_Handler* handler, _Method method) {
		std::cout << "UserEventLoop" << std::endl;
	}

private:
	esp_event_loop_handle_t createLoop(const esp_event_loop_args_t *event_loop_args) {
		esp_event_loop_handle_t handle = nullptr;
		ESP_ERROR_CHECK(esp_event_loop_create(event_loop_args, &handle));
		return handle;
	}

private:
	EvLoopPtr m_loop;
};

//template <
//			typename _EventBase,
//			typename _Handler,
//			typename _Method,
//			typename _LoopBase = EspSystemEventLoop
//		 >
//void registerHandler(_Handler* obj, _Method method,
//		_LoopBase& base = EspSystemEventLoop::Instance())
//{
//	base.template registerHandler<_EventBase>(obj, method);
//}

template <
			typename _EventBase,
			typename _Class,
			typename _Func,
			typename _LoopBase = EspSystemEventLoop
		 >
void registerHandler(_Class* obj, _Func func,
		_LoopBase& base = EspSystemEventLoop::Instance())
{
	base.template registerHandler<_EventBase>(obj, func);
}

template <
			typename _EventBase,
			typename _Func,
			typename _LoopBase = EspSystemEventLoop
		 >
void registerHandler(_Func func,
		_LoopBase& base = EspSystemEventLoop::Instance())
{
	base.template registerHandler<_EventBase>(func);
}



} /* namespace amosoft */


#endif /* COMPONENTS_CPP_EVENT_LOOP_ESPEVENTLOOP_H_ */
