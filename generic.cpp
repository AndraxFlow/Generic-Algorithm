#include <iostream> 
#include <random> 
#include <string> 
#include <ctime> 
//#include <map>

using namespace std;

double func(double x, double y) {
    return x * x + y * y - x * y + x + y;
}

vector <double> count_func_values(vector <string> population) {

    vector <double> values;

    string number1, fract1, number2, fract2;
    for (string individual : population) {
        char sign1 = individual[0], sign2 = individual[7];
        number1 = individual.substr(1, 6);
        fract1 = individual.substr(8, 6);
        number2 = individual.substr(14, 6);
        fract2 = individual.substr(20, 6);

        double num = 0, frac = 0;
        for (int i = 0; i < 6; i++) {
            num += (double)(number1[5 - i] - '0') * pow(2, i);
        }

        for (int i = 0; i < 6; i++) {
            frac += (1 / pow(2, i + 1)) * (double)(fract1[i] - '0');
        }

        double num1 = (num + frac) * pow(-1, sign1 - '0');

        num = 0, frac = 0;
        for (int i = 0; i < 6; i++) {
            num += (double)(number2[5 - i] - '0') * pow(2, i);
        }

        for (int i = 0; i < 6; i++) {
            frac += (1 / pow(2, i + 1)) * (double)(fract2[i] - '0');
        }

        double num2 = (num + frac) * pow(-1, sign2 - '0');

        double result = func(num1, num2);

        values.push_back(result);
    }


    return values;
}

string crossing(string parent1, string parent2) {
    int index = rand() % 26;
    string children = parent1.substr(0, index) + parent2.substr(index, 26 - index);

    return children;
}

vector <string> new_population(vector <string> parents) {
    vector <string> children;
    for (int i = 0; i < 7; i++) {
        int first = rand() % 7;
        int second = rand() % 7;

        string child1 = crossing(parents[first], parents[second]);
        string child2 = crossing(parents[second], parents[first]);


        children.push_back(child1);
        children.push_back(child2);
    }
    return children;
}

vector <string> selection(vector <string> generation, vector <int> score) {
    vector <string> parents;

    for (int i = 0; i < 7; i++) {
        int first = rand() % 14;
        int second = rand() % 14;

        if (score[first] > score[second]) {
            parents.push_back(generation[first]);
        }
        else {
            parents.push_back(generation[second]);
        }
    }

    return parents;
}

vector <int> fitness_individuals(vector <double> values) {
    vector <int> score;
    double max_score = -999999, min_score = 999999, sum_score = 0;

    for (int i = 0; i < values.size(); i++) {
        values[i] *= (-1);
        if (values[i] > max_score) {
            max_score = values[i];
        }
        if (values[i] < min_score) {
            min_score = values[i];
        }
    }

    for (int i = 0; i < values.size(); i++) {
        score.push_back((abs(min_score) / ((pow(max_score - values[i], 1) + 1) * 4)));
    }

    return score;
}

void mutation(vector <string>& population) { 
    int index = rand() % 5;
    int border1_of_mutation = rand() % 26;
    int border2_of_mutation = rand() % 26;

    if (border1_of_mutation - border2_of_mutation != 0 && abs(border1_of_mutation - border2_of_mutation) < 10) {
        for (int i = min(border1_of_mutation, border2_of_mutation); i < max(border1_of_mutation, border2_of_mutation); i++) {
            if (population[index][i] == '0') {
                population[index][i] = '1';
            }
            else {
                population[index][i] = '0';
            }
        }
    }
}

vector <string> init_population()
{
    vector <string> random_population;

    for (int individuals = 0; individuals < 14; individuals++) {
        string individual = "";
        for (int genetic_len = 0; genetic_len < 26; genetic_len++) {

            int num = rand() % 2;

            individual += to_string(num);
        }
        random_population.push_back(individual);
    }

    return random_population;
}


int main()
{
    srand(time(NULL));
    cout.precision(10);

    vector <string> population = init_population();
    double min = 999999;
    string best_genom = "";


    for (int i = 0; i < 10000; i++) {
        vector <double> values = count_func_values(population);
        mutation(population);
        vector <int> score = fitness_individuals(values);
        vector <string> parents = selection(population, score);
        vector <string> children = new_population(parents);

        double min_from_the_population = 999999;
        string genom = "";
        for (int i = 0; i < 14; i++) {
            if (values[i] < min_from_the_population) {
                min_from_the_population = values[i];
                genom = population[i];
            }

            if (min_from_the_population < min) {
                min = min_from_the_population;
                best_genom = genom;
            }
        }

        cout << "min from the generation = " << min_from_the_population << " with genom - " << genom << endl;

        population = children;
    }

    cout << "min = " << min << endl;
    cout << "best genom - " << best_genom << endl;
}