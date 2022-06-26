#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <future>
#include <cmath>
#include <python3.10/Python.h>
#include "pearson.h"
#include "processing.h"

std::vector<double> log_return(const char* f_name);
void vec_to_file(const std::vector<double>& dt, const char* f_name);

int main(int argc, char* argv[])
{
    auto neo1 = std::async(std::launch::async, log_return, argv[1]);
    auto neo2 = std::async(std::launch::async, log_return, argv[2]);
    std::vector<double> return1 = neo1.get();
    std::vector<double> return2 = neo2.get();
    
    const char* name = "log_return1.txt";
    vec_to_file(return1, name);

    const char* name2 = "log_return2.txt";
    vec_to_file(return2, name2);

    double p = pearson_co(return1, return1);
    printf("The Pearson Correlation Coefficient of 2 stocks is: %.4f\n", p);

    //Starting Python interpreter
    Py_Initialize();

    //Import libraries
    PyRun_SimpleString("import pandas as pd\n"
    "import numpy as np\n"
    "import matplotlib.pyplot as plt\n"
    "from statsmodels.tsa.stattools import adfuller");

    //Calculate cointegration
    





    //Building a string command to read the file
    const char * py_read1 = "df = pd.read_csv('";
    const char * py_read2 = "')\n";
    char buf[1024];
    snprintf(buf, sizeof(buf), "%s%s%s", py_read1, argv[1], py_read2);
    const char* str = buf;

    const char * py_read3 = "df2 = pd.read_csv('";
    char buf2[1024];
    snprintf(buf2, sizeof(buf2), "%s%s%s", py_read3, argv[2], py_read2);
    const char* str2 = buf2;

    //Plotting the graph
    PyRun_SimpleString(str);
    PyRun_SimpleString(str2);
    PyRun_SimpleString("plt.plot(df['Date'], df['Adj Close'], label='a')");
    PyRun_SimpleString("plt.plot(df2['Date'], df2['Adj Close'], label='b')");
    PyRun_SimpleString("plt.legend(loc='upper right')"); 
    PyRun_SimpleString("plt.show()\n");
    Py_Finalize();

    return 0;
}
