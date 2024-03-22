/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <variant>

//---------
template<class Data, class ErrorType>
using Result = std::variant<Data,  ErrorType>;

template<class ResultHandler, class ErrorHandler, class Data, class ErrorType>
void handleResult(Result<Data, ErrorType>&& result, ResultHandler resh, ErrorHandler errh) {
    std::visit([resh=std::move(resh), errh=std::move(errh)](auto&& arg){
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Data>) {
            resh(std::forward<decltype(arg)>(arg));
        } else if constexpr (std::is_same_v<T, ErrorType>) {
            errh(std::forward<decltype(arg)>(arg));
        }
    }, result);
}
//---------

//---------
enum ProcessErrorType{
    EError_1 = 0xF1,
    EError_2 = 0xF2,
    EError_3 = 0xFF
};

class ProcessData {
public:
    ProcessData(double data) :
        m_data(data)
    {}
    
    operator double(){ return m_data; }
private:
   double m_data;
};

using ProcessResult = Result<ProcessData, ProcessErrorType>;
//---------



ProcessResult process(bool param) {
	if (param) {
	    return 42.42;
	} else {
	    return EError_2;
	}
}

int main()
{
    auto res = process(true);
    
    handleResult(std::move(res), 
    [](ProcessData&& data){
        std::cout << "Data is obtained;";
    },
    [](ProcessErrorType&& err){
        std::cout << "Error is obtained;";
    }
    );

    return 0;
}

