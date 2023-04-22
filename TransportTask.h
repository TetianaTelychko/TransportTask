#ifndef TRANSPORTTASK_H
#define TRANSPORTTASK_H

#include <iostream>
#include <vector>
#include <algorithm>
using std::cout;
using std::cin;

class TransportTask{
private:
    int numberOfBatteries, numberOfWarehouses;
    std::vector<int> needs, reserves;
    std::vector<std::vector<int>> prices, quantity, values;

    int number(int min, int max, int multipleOf = 1){
        int number;
        do{
            number = rand() % max + min;
        }
        while(number % multipleOf != 0);

        return number;
    }
    void print(std::vector<int> &vector){
        for(int v : vector){
            cout << v << ' ';
        }
        cout << '\n';
    }
    void print(std::vector<std::vector<int>> &vector, std::vector<int> &additionalColumn, std::vector<int> &additionalLine){
        for(int i = 0; i < vector.size(); ++i){
            for(int j : vector[i]){
                if(j < 10){
                    cout << "   ";
                }
                else if(j < 100){
                    cout << "  ";
                }
                else if(j < 1000){
                    cout << " ";
                }
                cout << j << ' ';
            }
            cout << "  " << additionalColumn[i] << '\n';
        }
        for(int i : additionalLine){
            if(i < 10){
                cout << "   ";
            }
            else if(i < 100){
                cout << "  ";
            }
            else if(i < 1000){
                cout << " ";
            }
            cout << i << ' ';
        }
        cout << '\n';
    }
    void print(std::vector<std::vector<int>> &vector, int a){
        for(int i = 0; i < vector.size(); ++i){
            for(int j : vector[i]){
                if(j < 10){
                    cout << "   ";
                }
                else if(j < 100){
                    cout << "  ";
                }
                else if(j < 1000){
                    cout << " ";
                }
                cout << j << ' ';
            }
            if(i == vector.size() - 1){
                cout << "  " << a << '\n';
            }
            else{
                cout << '\n';
            }
        }
    }

    void fillingVectors(int B, int W){
        while(B != 0){
            needs.push_back(number(100, 500, 50));
            --B;
        }
        cout << "Вектор потреб : ";
        print(needs);

        while(W != 0){
            if(W % 2 == 0){
                reserves.push_back(number(10, 100, 10));
            }
            else{
                reserves.push_back(number(100, 1000, 10));
            }
            --W;
        }
        cout << "Вектор запасів : ";
        print(reserves);
    }
    void fillingPricesMatrix(){
        prices.reserve(numberOfWarehouses);
        for(int j = 0; j < numberOfWarehouses; ++j){
            std::vector<int> v;
            v.reserve(numberOfBatteries);
            prices.push_back(v);
            for(int i = 0; i < numberOfBatteries; ++i){
                prices[j].push_back(number(1, 9));
            }
        }
        cout << "Матриця цін перевезення однієї штуки з\n"
                "кожного складу до кожної батареї :\n";
        print(prices, reserves, needs);
    }

    void getMinInPrices(int &r, int &c){
        int min = 10; ///в матриці не може бути числа більше 9
        for(int i = 0; i < prices.size(); ++i){
            for(int j = 0; j < prices[i].size(); ++j){
                if(min > prices[i][j] && reserves[i] != 0 && needs[j] != 0){
                    min = prices[i][j];
                    r = i;
                    c = j;
                }
            }
        }
    }
    void fillingQuantityMatrix(){
        quantity.reserve(numberOfWarehouses);
        for(int j = 0; j < numberOfWarehouses; ++j){
            std::vector<int> v;
            v.reserve(numberOfBatteries);
            quantity.push_back(v);
            for(int i = 0; i < numberOfBatteries; ++i){
                quantity[j].push_back(0);
            }
        }
        int r, c;
        auto zeros = [](const std::vector<int>& v) {
            return std::all_of(v.begin(), v.end(), [](int i) { return i == 0; });
        };
        while(!zeros(needs) && !zeros(reserves)){
            getMinInPrices(r, c);
            if(needs[c] < reserves[r]){
                reserves[r] -= needs[c];
                quantity[r][c] = needs[c];
                needs[c] = 0;
            }
            else{
                needs[c] -= reserves[r];
                quantity[r][c] = reserves[r];
                reserves[r] = 0;
            }
        }
        cout << "Матриця кількість снарядів, які можна завезти з того\n"
                "чи іншого складу для кожної батареї таким чином,\n"
                "щоб вартість перевезення була мінімальна :\n";
        print(quantity, reserves, needs);
    }
    void fillingValuesMatrix(){
        values.reserve(numberOfWarehouses + 1);
        for(int j = 0; j < numberOfWarehouses; ++j){
            std::vector<int> v;
            v.reserve(numberOfBatteries);
            values.push_back(v);
            for(int i = 0; i < numberOfBatteries; ++i){
                if(quantity[j][i] != 0){
                    values[j].push_back(quantity[j][i] * prices[j][i]);
                }
                else{
                    values[j].push_back(0);
                }
            }
        }

        std::vector<int> v;
        int allSum = 0;
        for(int i = 0; i < numberOfBatteries; ++i){
            int sum = 0;
            for(int j = 0; j < numberOfWarehouses; ++j){
                sum += values[j][i];
            }
            v.push_back(sum);
            allSum += sum;
        }
        values.push_back(v);
        cout << "Матриця вартостей :\n";
        print(values, allSum);
    }

public:
    TransportTask(int B, int W) : numberOfBatteries(B), numberOfWarehouses(W){}

    void Start(){
        fillingVectors(numberOfBatteries, numberOfWarehouses);
        cout << '\n';
        fillingPricesMatrix();
        cout << '\n';
        fillingQuantityMatrix();
        cout << '\n';
        fillingValuesMatrix();
    }
};
#endif