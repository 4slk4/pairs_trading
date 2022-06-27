#include <string>
#include <iostream>
#include <python3.10/Python.h>
#include "pearson.h"
#include "processing.h"

int main(int argc, char* argv[])
{
    if (argc != 3){
        std::cout << "Usage: ./main <stock1>.csv <stock2>.csv" << std::endl;
        return -1;
    }
    else{
        //Process 2 files into 2 parallel arrays
        std::vector<double> return1 = adj_return(argv[1]);
        std::vector<double> return2 = adj_return(argv[2]);
        
        //Check if the two data have equal length; if not, exit 
        if (return1.size() != return2.size()){
            std::cout << "Two data have to be the same length" << std::endl;
            return -1;
        }

        double mean_r1 = mean(return1);
        double median_r1 = median(return1);
        double std_r1 = std_dev(return1);

        double mean_r2 = mean(return2);
        double median_r2 = median(return2);
        double std_r2 = std_dev(return2);

        std::cout << argv[1] << "   Mean: " << mean_r1 << "   Median: " << median_r1 << "   Standard Deviation: " << std_r1 << std::endl;
        std::cout << argv[2] << "   Mean: " << mean_r2 << "   Median: " << median_r2 << "   Standard Deviation: " << std_r2 << std::endl;
        std::cout << std::endl;
        
        //Calculate the Pearson Correlation Coefficient
        std::vector<double> log_return1 = log_return(return1);
        std::vector<double> log_return2 = log_return(return2);
        double p_co = pearson_co(log_return1, log_return2);
        printf("The Pearson Correlation Coefficient of 2 stocks is: %.6f\n", p_co);

        /*PYTHON EMBEDDING */
        
        // Starting Python interpreter
        Py_Initialize();

        //Import libraries
        PyRun_SimpleString("import pandas as pd\n"
        "import numpy as np\n"
        "import matplotlib.pyplot as plt\n"
        "from statsmodels.tsa.stattools import adfuller");

        //Building a string command to read the file
        const char * py_read1 = "df = pd.read_csv('";
        const char * py_end = "')\n";
        char buf[1024];
        snprintf(buf, sizeof(buf), "%s%s%s", py_read1, argv[1], py_end);
        const char* str = buf;

        const char * py_read3 = "df2 = pd.read_csv('";
        char buf2[1024];
        snprintf(buf2, sizeof(buf2), "%s%s%s", py_read3, argv[2], py_end);
        const char* str2 = buf2;

        //Read the file
        PyRun_SimpleString(str);
        PyRun_SimpleString(str2);

        //Calculate cointegration
        PyRun_SimpleString("spread_adf = adfuller(df['Adj Close'] - df2['Adj Close'])[1]");
        PyRun_SimpleString("print(f'The Augmented Dickey-Fuller test produces the P-Value: {spread_adf:.6f}')");
        PyRun_SimpleString("if spread_adf < 0.05:\n"
                            "   print(f'Since P-Value={spread_adf:.6f} < 0.05 -> two stocks are cointegrated')\n"
                            "else:\n"
                            "   print(f'Since P-Value={spread_adf:.6f} >= 0.05 -> two stocks are not cointegrated')");


        char choice;
        std::cout << "Do you want to see the graph? (y/n): ";
        std::cin >> choice;
        if (std::tolower(choice) == 'y'){
            //Normalize 'Adj Close' column with formula (x - mean(x))/ std(x)
            PyRun_SimpleString("df['Adj Close'] = (df['Adj Close'] - df['Adj Close'].mean())/df['Adj Close'].std()");
            PyRun_SimpleString("df2['Adj Close'] = (df2['Adj Close'] - df2['Adj Close'].mean())/df2['Adj Close'].std()");

            //Plotting the graph
            const char * py_plot1 = "plt.plot(df['Date'], df['Adj Close'], label='";
            char buf3[1024];
            snprintf(buf3, sizeof(buf3), "%s%s%s", py_plot1, argv[1], py_end);
            const char* plot1 = buf3;

            const char * py_plot2 = "plt.plot(df2['Date'], df2['Adj Close'], label='";
            char buf4[1024];
            snprintf(buf4, sizeof(buf4), "%s%s%s", py_plot2, argv[2], py_end);
            const char* plot2 = buf4;

            PyRun_SimpleString(plot1);
            PyRun_SimpleString(plot2);
            PyRun_SimpleString("plt.legend(loc='upper right')"); 
            PyRun_SimpleString("plt.show()\n");
        }
        
        Py_Finalize();
    }

    return 0;
}
