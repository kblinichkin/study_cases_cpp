#include <freertos/FreeRTOS.h>
#include <iostream>

//#include <espeventloop.h>
#include <espeventloop.h>
//#include <espevent.h>
//#include <esp_event.h>
//#include <espcomponent.h>

using namespace amosoft;

class EspApplication {
public:
	static EspSystemEventLoop& MainEventLoop()
	{
		return EspSystemEventLoop::Instance();
	}
};

class A {
public:
	void ddd(const EspCustomEvent& event) {
		std::cout << "HaHaHa!" <<std::endl;
	}

	void fff(const EspCustomEvent& event) {

	}

	void rrr() {
		registerHandler<EspCustomEvent>(this, &A::fff);
	}
};

//template <class T> struct _arg_type { using type = T; };
//template <class Sig> struct _func_type;//{ using type = __func_type;};
//template <class _Func, class Arg>
//struct _func_type<_Func(Arg)>:_arg_type<Arg> {
//
//};
//
//void test_test_test(int*) {}
//
//template <class _Event, class _Func>
//class is_correct {
//	typedef char one;
//	struct two {char x[2];};
//
//	template<class E, class F> static one test(typeof(1));
//	template<class E, class F> static two test(...);
//
//	enum {value = sizeof(test<_Event, _Func>(0)) == sizeof(char)};
//};
//template <class _Event, class _Class, class _Func>
//void test_types(_Class* obj, _Func func) {
//	static_assert(std::is_same <
//									typename _func_type<_Func>::type,
//									_Func
//								>::value, "!!!!!");
//}

void testFunc()
{

	esp_event_loop_args_t args;
	EspEventLoop loop(&args);

	A a;

	::registerHandler<EspCustomEvent>(&a, &A::ddd);

	::registerHandler<EspCustomEvent>(&a, &A::ddd, loop);

//	EspCustomEvent<> event = new EspCustomEvent<>();
//	::postEvent<EspCustomEvent>(event);
//	::postEvent<EspCustomEvent>(event, loop);
}

extern "C" void app_main(void)
{
	testFunc();


//	EspApplication::Init(); // register events
	//EventLoopBase* m_base = EspApplication::SystemEventLoop();

//	EspComponent comp1;
//	EspComponent comp2;

//	EspEventLoop loop;
//	comp1.changeEventLoop(loop);
//	comp2.changeEventLoop(loop);
}

