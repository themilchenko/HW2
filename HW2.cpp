#define _USE_MATH_DEFINES

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

enum ELEMENT_TYPE /*типы элементов строки*/
{
    /*группа констант*/
    ET_VALUE,          /*число         */

    /*группа операций без скобок*/
    ET_POW,            /*возведение в степень  */
    ET_MULT,           /*умножение    */
    ET_DIV,            /*деление      */
    ET_PLUS,           /*плюс         */
    ET_MINUS,          /*минус         */

    /*группа операций со скобками*/
    ET_BRACKET_LEFT,   /*левая  скобка */
    ET_BRACKET_RIGHT,  /*правая скобка */
    ET_SIN,            /*синус         */
    ET_COS,            /*косинус       */
    ET_TAN,            /*тангенс       */
    ET_COTAN,          /*котангенс     */
    ET_EXP,            /*экспонента    */
    ET_SQRT,           /*корень квадратный     */

    /*группа операций, требующие дополнительные действия*/
    ET_X,              /*переменная Х          */
};

/* структура элементов строки                                    */
struct data
{
    ELEMENT_TYPE element_type;  /*тип элемента (ELEMENT_TYPE для упрощения восприятия)*/
    double value;               /*значение текстового поля, если оно число*/
    std::string str_value;      /*текстовое значение числа (для удобства в поиске ошибок)*/
};

void help()
{
    std::cout << "============================================================================================\n"
              << "Welcome to the calculator!\n"
              << "There are functions and operations you may use:\n"
              << "1. Operations: plus (+), minus (-), division (/), multiplication(*) and unary minus (- 'some digit');\n"
              << "2. Brackets: open bracket ('(') and close bracket (')');\n"
              << "3. Trigonometry functions: sinus (sin('some values')), cosinus (cos('something'), tangens (tg('something'),"
              << " cotangens (cot('something');\n"
              << "4. Functions: exhibition (exp('something')), square root (sqrt('something')) and power (('something')^('something');\n"
              << "5. Constants: epsilon (e) and the number pi (pi).\n"
              << "6. You can enter the variable 'x' in your expression and after this assign value to it.\n"
              << "============================================================================================\n";
}

/*поиск позиции первого положения оператора*/
int find_position_first_operations(std::string input, std::string& str_value, ELEMENT_TYPE& operation_type, double& value, int& operation_length, std::vector<data> field)
{
    int position = std::string::npos; /*номер искомой позиции (изначально это максимальное положительное число) */
    operation_length = 0;             /*кол-во символов в операторе*/
    value = 0;

    if (input.find("*") < position)  /*умножение*/
    {
        position = input.find("*");
        operation_type = ET_MULT;
        operation_length = 1;
    }

    if (input.find("/") < position) /*деление*/
    {
        position = input.find("/");
        operation_type = ET_DIV;
        operation_length = 1;
    }

    if (input.find("+") < position) /*сложение*/
    {
        position = input.find("+");
        operation_type = ET_PLUS;
        operation_length = 1;
    }

    if (input.find("-") < position) /*вычитание*/
    {
        position = input.find("-");
        operation_type = ET_MINUS;
        operation_length = 1;
    }

    if (input.find("pi") < position) /*константа пи*/
    {
        position = input.find("pi");
        operation_type = ET_VALUE;
        operation_length = 2;
        value = M_PI;
    }

    if (input.find("exp(") < position) /*функция экспонента*/
    {
        position = input.find("exp(");
        operation_type = ET_EXP;
        operation_length = 4;
    }

    if (input.find("e") < position) /*константа епсилон*/
    {
        position = input.find("e");
        operation_type = ET_VALUE;
        operation_length = 1;
        value = M_E;
    }

    if (input.find("^") < position) /*деление*/
    {
        position = input.find("^");
        operation_type = ET_POW;
        operation_length = 1;
    }

    if (input.find("sqrt(") < position) /*функция квадратный корень*/
    {
        position = input.find("sqrt(");
        operation_type = ET_SQRT;
        operation_length = 5;
    }

    if (input.find("sin(") < position) /*функция синус*/
    {
        position = input.find("sin(");
        operation_type = ET_SIN;
        operation_length = 4;
    }

    if (input.find("cos(") < position) /*функция косинус*/
    {
        position = input.find("cos(");
        operation_type = ET_COS;
        operation_length = 4;
    }

    if (input.find("ctg(") < position) /*функция котангенс*/
    {
        position = input.find("ctg(");
        operation_type = ET_COTAN;
        operation_length = 4;
    }

    if (input.find("tg(") < position) /*функция тангенс*/
    {
        position = input.find("tg(");
        operation_type = ET_TAN;
        operation_length = 3;
    }

    if (input.find("(") < position) /*открывающаяся скобка*/
    {
        position = input.find("(");
        operation_type = ET_BRACKET_LEFT;
        operation_length = 1;
    }

    if (input.find(")") < position) /*закрывающаяся скобка*/
    {
        position = input.find(")");
        operation_type = ET_BRACKET_RIGHT;
        operation_length = 1;
    }

    if (input.find("x") < position) /*переменная икс*/
    {
        position = input.find("x");
        operation_type = ET_X;
        operation_length = 1;
    }

    if ((position > 0) && (input.size() > 0)) /*считывание числа*/
    {
        str_value = input.substr(0, position);
        value = std::stod(str_value);
        operation_length = str_value.size();
        operation_type = ET_VALUE;
    }

    if ((position == -1) && (input.size() > 0)) /*считывание последнего числа в варажении*/
    {
        str_value = input.substr(0, input.size());
        value = std::stod(str_value);
        operation_length = str_value.size();
        operation_type = ET_VALUE;
        position = input.size();
    }

    return position;
}

void unary(std::vector<data>& field) /*проверка на наличие унарного минуса*/
{
    if (field[0].element_type == ET_MINUS && field[1].element_type == ET_VALUE)  /*если строка начинается с минуса*/
    {
        field[1].value = -field[1].value;
        field.erase(field.begin());
    }
    for (int i = 1; i < field.size() - 1; i++)   /*рассматриваю все случае унарного минуса*/
    {
        if (field[i].element_type == ET_MINUS &&
            (field[i - 1].element_type == ET_PLUS ||
             field[i - 1].element_type == ET_MINUS ||
             field[i - 1].element_type == ET_DIV ||
             field[i - 1].element_type == ET_MULT ||
             field[i - 1].element_type == ET_BRACKET_LEFT))
        {
            if (field[i + 1].element_type == ET_VALUE)
            {
                field[i + 1].value = -field[i + 1].value;
                field.erase(field.begin() + i);
            }
        }
    }
}

/*ковретация строки ввода в массив элементов*/
int input_to_vector_data(std::string input, std::vector<data>& elements)
{
    data elemet;                    /*структура нового элемента*/
    int operation_length = 0;       /*возвращаемое значение длины элемента в символах*/
    double value = 0;               /*возвращаемое значение*/
    std::string string_value;
    ELEMENT_TYPE operation_type;    /*возвращаемое значение типа элемента*/

    /*пока не найдем std::string::npos*/
    while ((find_position_first_operations(input, string_value, operation_type, value, operation_length, elements) != std::string::npos))
    {
        if (operation_type == ET_VALUE) /*проверка, что найденый символ - число*/
        {
            elemet.element_type = ET_VALUE;
            elemet.str_value = string_value;
            elemet.value = value;
        }
        else /*обработка всех остальных операторов*/
        {
            elemet.element_type = operation_type;
            elemet.value = 0;
        }

        input.erase(0, operation_length);   /*очистка найденного символа или опретатора из строки*/
        elements.push_back(elemet);         /*вставка нового элемента*/

    }

    return 0;
}

/*поиск скобок*/
bool find_brackets(std::vector<data>& field,
                   int& pos_open_bracket,
                   int& pos_close_bracket)/*входной массив элементов выражения*/
{
    pos_close_bracket = -1;/*номер элемента положения правой скобки*/
    pos_open_bracket = -1;/*номер элемента положения левой скобки или операции со скобкой*/

    for (int i = (field.size() - 1); i >= 0; i--)               /*ищем последнюю открывающуюся скобку*/
        if (field[i].element_type == ET_BRACKET_LEFT ||
            field[i].element_type == ET_COS ||
            field[i].element_type == ET_COTAN ||
            field[i].element_type == ET_EXP ||
            field[i].element_type == ET_SIN ||
            field[i].element_type == ET_SQRT)
        {
            pos_open_bracket = i;
            break;
        }

    if (pos_open_bracket == -1)       /*если не нашлась открывающаяся скобка, смотрим, есть ли закрывающиеся*/
    {
        for (int i = 0; i < field.size(); i++) /*ищем закрывающуюся скобку относительно открывающейся*/
            if (field[i].element_type == ET_BRACKET_RIGHT)
            {
                pos_close_bracket = i;
                break;
            }
    }
    else         /*иначе ищем закрывающуюся скобку относительно открывающейся*/
    {

        for (int i = pos_open_bracket; i < field.size(); i++)
            if (field[i].element_type == ET_BRACKET_RIGHT)
            {
                pos_close_bracket = i;
                break;
            }

    }

    if ((pos_open_bracket == -1) && (pos_close_bracket == -1) && field.size() > 1)
    {
        pos_open_bracket = 0;
        pos_close_bracket = field.size() - 1;
        return false;/*если скобка не найдена*/
    }
        /*неверное кол-во открытых и закрытых скобок*/
    else if (((pos_open_bracket == -1) && (pos_close_bracket != -1)) || ((pos_open_bracket != -1) && (pos_close_bracket == -1)))
    {
        std::cout << "Something wrong with brackets\n";
        /*аварийный выход из программы*/
        exit(1);
        return false;
    }

    return true;/*если скобка найдена*/

}


/*функция возвращения первой приоритетной операции*/
int new_priority(std::vector<data>& field, int left, int right) /* будет возвращать знак*/
{

    for (int i = left + 1; i < right; i++)    /*возведение в степень*/
    {
        if (field[i].element_type == ET_POW)
            return i;
    }

    for (int i = left + 1; i < right; i++)    /*умножение*/
    {
        if (field[i].element_type == ET_MULT)
            return i;
    }

    for (int i = left + 1; i < right; i++) /*деление*/
    {
        if (field[i].element_type == ET_DIV)
            return i;
    }

    for (int i = left + 1; i < right; i++) /*сложение*/
    {
        if (field[i].element_type == ET_PLUS)
            return i;
    }

    for (int i = left + 1; i < right; i++) /*вычитание*/
    {
        if (field[i].element_type == ET_MINUS)
            return i;
    }

    return -1;
}


/*выполнение операции с последующим упрощением выражения*/
int run_operation(std::vector<data>& field, int position_operation)
{
    double rezult_value = 0;
    /*выполняем операцию: */
    if (field[position_operation].element_type == ET_POW)
        rezult_value = pow(field[position_operation - 1].value, field[position_operation + 1].value);
    else if (field[position_operation].element_type == ET_MULT)
        rezult_value = field[position_operation - 1].value * field[position_operation + 1].value;
    else if (field[position_operation].element_type == ET_PLUS)
        rezult_value = field[position_operation - 1].value + field[position_operation + 1].value;
    else if (field[position_operation].element_type == ET_MINUS)
        rezult_value = field[position_operation - 1].value - field[position_operation + 1].value;
    else if (field[position_operation].element_type == ET_DIV)
        rezult_value = field[position_operation - 1].value / field[position_operation + 1].value;

    /*удаляем две первые позиции подсчитываемого выражения*/
    field.erase(field.begin() + position_operation, field.begin() + position_operation + 2);
    field[position_operation - 1].value = rezult_value; /*присваиваем третьей ячейке результат выполненнго выражения*/

    return 0;
}

void erase_brackets(std::vector<data>& field, int& open_br, int& close_br, int is_br)
{
    if (((close_br - open_br) == 2) && (is_br == 1))       /*если в скобках одно число или в скобках какая-то функция*/
    {
        if (field[open_br].element_type == ET_SIN)
            field[open_br + 1].value = sin(field[open_br + 1].value);
        else if (field[open_br].element_type == ET_COS)
            field[open_br + 1].value = cos(field[open_br + 1].value);
        else if (field[open_br].element_type == ET_TAN)
            field[open_br + 1].value = tan(field[open_br + 1].value);
        else if (field[open_br].element_type == ET_COTAN)
            field[open_br + 1].value = 1.0 / tan(field[open_br + 1].value);
        else if (field[open_br].element_type == ET_EXP)
            field[open_br + 1].value = exp(field[open_br + 1].value);
        else if (field[open_br].element_type == ET_SQRT)
            field[open_br + 1].value = sqrt(field[open_br + 1].value);

        field.erase(field.begin() + close_br);                           /*стирааем правую скобку*/
        field.erase(field.begin() + open_br);                            /*стираем левую скобку  */

        unary(field);                                                    /*после этого проверяем выражение на унарность*/

        find_brackets(field, open_br, close_br);                         /*обновляем позиции скобок в выражении*/
    }
}

int calculate(std::vector<data>& field)
{
    int pos_operation = -1;

    int close_br = 0;                                                            /*ищем первую закрывающуюся скобку*/
    int open_br = 0;                                                             /*ищем первую открывающуюся скобку*/
    int is_br = 0;                                                               /*признак наличия скобок*/

    unary(field);                                                                /*проверяем унарность*/

    is_br = find_brackets(field, open_br, close_br);                             /*поиск положения скобок или границ*/

    erase_brackets(field, open_br, close_br, is_br);

    while ((close_br - open_br) > 1)                                              /*пока между скобками не одно число------*/
    {
        pos_operation = new_priority(field, open_br, close_br);                    /*определяем приоритетность внутри скобок*/

        if (pos_operation != -1)                                                  /*если нашли приоритетную операцию-------*/
            run_operation(field, pos_operation);                                  /*выполняем приоритетную операцию--------*/

        is_br = find_brackets(field, open_br, close_br);                          /*поиск положения скобок или границ*/

        erase_brackets(field, open_br, close_br, is_br);
    }

    return 0;
}

int check_x(std::vector<data>& field) /*проверка на наличие переменной*/
{
    int counter = 0;
    for (auto i : field)
        if (i.element_type == ET_X)
            counter++;
    return counter;
}

void replace_x(std::vector<data>& field, double x) /*замена переменной на число*/
{
    for (auto& i : field)
        if (i.element_type == ET_X)
        {
            i.value = x;
            i.element_type = ET_VALUE;
        }
}

void check_mistake(std::vector<data>& field) /*если перед скобками, после скобок или какой-то функцией не стоит знак умножить*/
{

    int counter_of_ET = 0;

    for (int i = 0; i< field.size(); i++)
        if ((field[i].element_type != ET_VALUE) && (field[i + 1].element_type != ET_VALUE))
        {
            if ((field[i].element_type == ET_BRACKET_LEFT && field[i + 1].element_type == ET_BRACKET_LEFT) ||
                (field[i].element_type == ET_BRACKET_RIGHT && field[i + 1].element_type == ET_BRACKET_RIGHT) ||
                    (field[i].element_type == ET_MINUS && field[i + 1].element_type == ET_BRACKET_LEFT) ||
                    (field[i].element_type == ET_BRACKET_RIGHT && field[i + 1].element_type == ET_POW) ||
                    (field[i].element_type == ET_POW && field[i + 1].element_type == ET_BRACKET_LEFT) ||
                    (field[i].element_type == ET_MINUS && (field[i + 1].element_type == ET_SIN || field[i + 1].element_type == ET_COS
                    || field[i + 1].element_type == ET_TAN || field[i + 1].element_type == ET_COTAN ||
                            field[i + 1].element_type == ET_SQRT || field[i + 1].element_type == ET_EXP
                            || field[i + 1].element_type == ET_POW)))
                continue;
            else
                counter_of_ET++;

            if (field[i].element_type == ET_VALUE || field[i].element_type == ET_X)
                counter_of_ET = 0;
        }

    if (counter_of_ET > 2)
    {
        std::cout << "Incorrect input.";
        exit(0);
    }

    for (int i = 0; i < field.size() - 1; i++)      /*после числа на поставлен знак умножить*/
        if ((field[i].element_type == ET_VALUE) &&
            (field[i + 1].element_type == ET_BRACKET_LEFT ||
             field[i + 1].element_type == ET_COS ||
             field[i + 1].element_type == ET_SIN ||
             field[i + 1].element_type == ET_TAN ||
             field[i + 1].element_type == ET_COTAN ||
             field[i + 1].element_type == ET_EXP ||
             field[i + 1].element_type == ET_SQRT ||
             field[i + 1].element_type == ET_X))
        {
            std::cout << "Don't forget to write '*' before function or brackets!";
            exit(0);
        }

    for (int i = 0; i < field.size() - 1; i++)      /*после закрывающейся скобки не стоит знак умножить*/
        if (field[i].element_type == ET_BRACKET_RIGHT &&
            (field[i + 1].element_type == ET_BRACKET_LEFT ||
             field[i + 1].element_type == ET_VALUE ||
             field[i + 1].element_type == ET_SIN ||
             field[i + 1].element_type == ET_COS ||
             field[i + 1].element_type == ET_TAN ||
             field[i + 1].element_type == ET_COTAN ||
             field[i + 1].element_type == ET_EXP ||
             field[i + 1].element_type == ET_SQRT ||
             field[i + 1].element_type == ET_X))
        {
            std::cout << "Don't forget to write '*' before function or brackets or after its!";
            exit(0);
        }

    for (int i = 0; i < field.size(); i++)
        if (field[i].element_type == ET_VALUE)
        {
            int counter_of_points = 0;
            for (int j = 0; j < field[i].str_value.size(); j++)
                if (field[i].str_value[j] == '.')
                    counter_of_points++;

            if (counter_of_points > 1)
            {
                std::cout << "There aren't one point in the value.";
                exit(0);
            }
        }

}

int main()
{
    int command = 0;

    do
    {
        command = -1;
        std::cout << "Enter '0' to see instruction;\n"
                  << "Enter '1' to calculate an expression;\n"
                  << "Enter '2' to finish\n"
                  << "Enter the command you want to use: ";

        std::cin >> command;

        if (command == 1)
        {
            std::vector<data> field;
            std::string input = "";
            std::cin.ignore();
            std::cout << "Enter the expression: ";
            std::getline(std::cin, input);
            std::string final = input;

            input.erase(std::remove(input.begin(), input.end(), ' '), input.end());

            std::transform(input.begin(), input.end(), input.begin(), ::tolower);

            input_to_vector_data(input, field);

            unary(field);

            check_mistake(field);

            if (check_x(field) != 0)
            {
                double x;
                std::cout << "Enter the value of x: ";
                std::cin >> x;
                replace_x(field, x);
            }

            int current = field.size();

            calculate(field);

            if (current == field.size())
            {
                std::cout << "Incorrect input.";
                exit(0);
            }

            std::cout << "=================================================\n";
            std::cout << final << " = " << field[0].value << std::endl;
            std::cout << "=================================================\n";
        }

        if (command == 0)
            help();

    } while (command != 2);

    return 0;
}
