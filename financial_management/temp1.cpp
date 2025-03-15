#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

int flag{0};

const std::string NEW_CATEGORY = "new_category";
const std::string ADD_EXPENSE = "add_expense";
const std::string REMOVE_EXPENSE = "remove_expense";
const std::string REPORT = "report";
const char SLASH = '/';

struct SubCategory {
    std::string name;
    std::vector<int> expenses;

    SubCategory(const std::string &subcategory_name_parametr)
        : name(subcategory_name_parametr) {}

    void display_info() const;
    void add_expense(int expense);
    void remove_expense();
};

void SubCategory::display_info() const {
    int total_subcategory_expenses{std::accumulate(expenses.begin(), expenses.end(), 0)};
    std::cout << "  - " << name << ": " << total_subcategory_expenses << std::endl;
}

void SubCategory::add_expense(int expense) {
    expenses.push_back(expense);
}

void SubCategory::remove_expense() {
    if (!expenses.empty()) {
        expenses.pop_back();
    } else {
        std::cout << "No expenses to remove in subcategory: " << name << "\n";
    }
}

struct Category {
    std::string name;
    std::vector<SubCategory> subcategories;
    std::vector<int> expenses;

    Category(const std::string &category_name_parametr)
        : name(category_name_parametr) {}

    void display_info();
    void display_info_with_filter(int filter_amount);
    void add_subcategory(const SubCategory &subcategory);
    void add_expense(int expense);
    void remove_expense();
    void sort_subcategories();
};

void Category::sort_subcategories() {
    std::sort(subcategories.begin(), subcategories.end(), [](const SubCategory &a, const SubCategory &b) {
        int total_expenses_a = std::accumulate(a.expenses.begin(), a.expenses.end(), 0);
        int total_expenses_b = std::accumulate(b.expenses.begin(), b.expenses.end(), 0);

        if (total_expenses_a != total_expenses_b) {
            return total_expenses_a > total_expenses_b;
        }
        return a.name < b.name;
    });
}

void Category::add_expense(int expense) {
    expenses.push_back(expense);
}

void Category::remove_expense() {
    if (!expenses.empty()) {
        expenses.pop_back();
    } else {
        std::cout << "No expenses to remove in category: " << name << "\n";
    }
}

void Category::display_info() {
    if (!subcategories.empty()) {
        int total_category_expenses = std::accumulate(expenses.begin(), expenses.end(), 0);

        if (flag) {
            std::cout << "----------" << std::endl;
        }
        std::cout << "Category: " << name << "\n";
        flag = 1;

        int sum_sub = std::accumulate(subcategories.begin(), subcategories.end(), 0,
                                      [](int sum, const SubCategory &sub) {
                                          return sum + std::accumulate(sub.expenses.begin(), sub.expenses.end(), 0);
                                      });

        total_category_expenses += sum_sub;
        std::cout << "Total expenses: " << total_category_expenses << std::endl;
        sort_subcategories();
        std::cout << "Subcategories expenses:" << std::endl;
        for (const auto &subcategory : subcategories) {
            subcategory.display_info();
        }
    } else {
        if (flag) {
            std::cout << "----------" << std::endl;
        }
        std::cout << "Category: " << name << "\n";
        flag = 1;
        std::cout << "Subcategories expenses:" << std::endl;
    }
}

void Category::display_info_with_filter(int filter_amount) {
    if (!subcategories.empty()) {
        int total_category_expenses = std::accumulate(expenses.begin(), expenses.end(), 0);
        if (flag) {
            std::cout << "----------" << std::endl;
        }
        std::cout << "Category: " << name << std::endl;
        flag = 1;

        int sum_sub = std::accumulate(subcategories.begin(), subcategories.end(), 0,
                                      [](int sum, const SubCategory &sub) {
                                          return sum + std::accumulate(sub.expenses.begin(), sub.expenses.end(), 0);
                                      });

        total_category_expenses += sum_sub;
        std::cout << "Total expenses: " << total_category_expenses << std::endl;
        sort_subcategories();
        std::cout << "Subcategories expenses:" << std::endl;
        for (const auto &subcategory : subcategories) {
            int temp_sum = std::accumulate(subcategory.expenses.begin(), subcategory.expenses.end(), 0);
            if (temp_sum > filter_amount) {
                subcategory.display_info();
            }
        }
    } else {
        if (flag) {
            std::cout << "----------" << std::endl;
        }
        std::cout << "Category: " << name << "\n";
        flag = 1;
        std::cout << "Subcategories expenses:" << std::endl;
    }
}

void Category::add_subcategory(const SubCategory &subcategory) {
    subcategories.push_back(subcategory);
}

struct FinancialManagementApp {
    std::string name;
    std::vector<Category> categories;

    void display_info();
    void display_info_with_filter(int &filter_amount);
    void add_category(const Category &category);
    void add_expense_to_category(const std::string &category_name, int amount);
    void add_expense_to_subcategory(const std::string &category_name, const std::string &subcategory_name, int amount);
    void pop_subcategory(const std::string &category_name, const std::string &subcategory_name);
    void pop_category(const std::string &category_name);
    void sort_categories();
};

void FinancialManagementApp::sort_categories() {
    std::sort(categories.begin(), categories.end(), [](const Category &a, const Category &b) {
        return a.name < b.name;
    });
}

void FinancialManagementApp::add_expense_to_subcategory(const std::string &category_name, const std::string &subcategory_name, int amount) {
    for (auto &category : categories) {
        if (category.name == category_name) {
            for (auto &subcategory : category.subcategories) {
                if (subcategory.name == subcategory_name) {
                    subcategory.add_expense(amount);
                    return;
                }
            }
            std::cout << "Subcategory " << subcategory_name << " not found in category " << category_name << ".\n";
            return;
        }
    }
    std::cout << "Category " << category_name << " not found.\n";
}

void FinancialManagementApp::pop_subcategory(const std::string &category_name, const std::string &subcategory_name) {
    for (auto &category : categories) {
        if (category.name == category_name) {
            for (auto &subcategory : category.subcategories) {
                if (subcategory.name == subcategory_name) {
                    subcategory.remove_expense();
                    return;
                }
            }
            std::cout << "Subcategory " << subcategory_name << " not found in category " << category_name << ".\n";
            return;
        }
    }
    std::cout << "Category " << category_name << " not found.\n";
}

void FinancialManagementApp::add_expense_to_category(const std::string &category_name, int amount) {
    for (auto &category : categories) {
        if (category.name == category_name) {
            category.add_expense(amount);
            return;
        }
    }
    std::cout << "Category " << category_name << " not found.\n";
}

void FinancialManagementApp::pop_category(const std::string &category_name) {
    for (auto &category : categories) {
        if (category.name == category_name) {
            category.remove_expense();
            return;
        }
    }
    std::cout << "Category " << category_name << " not found.\n";
}

void FinancialManagementApp::display_info() {
    if (!categories.empty()) {
        for (auto &category : categories) {
            category.display_info();
        }
        std::cout << "**********" << std::endl;
    } else {
        std::cout << "No categories available." << std::endl;
    }
}

void FinancialManagementApp::display_info_with_filter(int &filter_amount) {
    if (!categories.empty()) {
        for (auto &category : categories) {
            category.display_info_with_filter(filter_amount);
        }
        std::cout << "**********" << std::endl;
    } else {
        std::cout << "No categories available." << std::endl;
    }
}

void FinancialManagementApp::add_category(const Category &category) {
    categories.push_back(category);
}

void new_category(FinancialManagementApp &app, const std::string &rest_of_input) {
    std::istringstream other_parsed_input(rest_of_input);
    std::string category_name;
    other_parsed_input >> category_name;

    Category new_category(category_name);

    std::string subcategory_name;
    while (other_parsed_input >> subcategory_name) {
        SubCategory new_subcategory(subcategory_name);
        new_category.add_subcategory(new_subcategory);
    }

    app.add_category(new_category);
}

void add_expense(FinancialManagementApp &app, const std::string &rest_of_input) {
    std::string::size_type identifier = rest_of_input.find(SLASH);
    if (identifier != std::string::npos) {
        std::string category_name = rest_of_input.substr(0, identifier);
        category_name.erase(category_name.begin(), std::find_if(category_name.begin(), category_name.end(), [](unsigned char ch) {
                                return !std::isspace(ch);
                            }));

        std::string subcategory_and_amount = rest_of_input.substr(identifier + 1);

        std::istringstream iss(subcategory_and_amount);
        std::string subcategory_name;
        int amount;

        iss >> subcategory_name >> amount;

        app.add_expense_to_subcategory(category_name, subcategory_name, amount);
    } else {
        std::string category_name;
        std::istringstream other(rest_of_input);

        other >> category_name;
        int amount{0};
        other >> amount;
        app.add_expense_to_category(category_name, amount);
    }
}

void remove_expense(FinancialManagementApp &app, const std::string &rest_of_input) {
    std::string::size_type identifier = rest_of_input.find(SLASH);
    if (identifier != std::string::npos) {
        std::string category_name = rest_of_input.substr(0, identifier);
        category_name.erase(category_name.begin(), std::find_if(category_name.begin(), category_name.end(), [](unsigned char ch) {
                                return !std::isspace(ch);
                            }));

        std::string other = rest_of_input.substr(identifier + 1);

        std::istringstream iss(other);
        std::string subcategory_name;

        iss >> subcategory_name;

        app.pop_subcategory(category_name, subcategory_name);
    } else {
        std::string category_name;
        std::istringstream other(rest_of_input);

        other >> category_name;
        app.pop_category(category_name);
    }
}

void report(FinancialManagementApp &app, const std::string &rest_of_input) {
    if (rest_of_input.empty()) {
        app.display_info();
    } else {
        int filter_amount;
        std::istringstream filter(rest_of_input);
        filter >> filter_amount;
        app.display_info_with_filter(filter_amount);
    }
}

void parse_input(FinancialManagementApp &app, const std::string input) {
    std::string command;
    std::string rest_of_input;
    std::istringstream first_parsed_input(input);

    first_parsed_input >> command;
    std::getline(first_parsed_input, rest_of_input);

    if (command == NEW_CATEGORY) {
        new_category(app, rest_of_input);
    } else if (command == ADD_EXPENSE) {
        add_expense(app, rest_of_input);
    } else if (command == REMOVE_EXPENSE) {
        remove_expense(app, rest_of_input);
    } else if (command == REPORT) {
        report(app, rest_of_input);
    } else {
        std::cout << "Unknown command: " << command << "\n";
    }
}

int main() {
    FinancialManagementApp app;
    std::string input;

    while (true) {
        std::cout << "Enter a command (or type 'exit' to quit): ";
        flag = 0;  // Reset flag for each input
        std::getline(std::cin, input);
        if (input == "exit") {
            break;
        }

        parse_input(app, input);
    }

    return 0;
}