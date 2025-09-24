/**
 * C++ Classes and Objects - Modern OOP Features
 * Covers classes, inheritance, polymorphism, and modern C++ OOP features
 */

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <type_traits>
#include <algorithm>

namespace classes {

// Basic class with modern features
class Person {
private:
    std::string name_;
    int age_;
    
public:
    // Default constructor
    Person() : name_("Unknown"), age_(0) {
        std::cout << "Person default constructor\n";
    }
    
    // Parameterized constructor
    Person(const std::string& name, int age) : name_(name), age_(age) {
        std::cout << "Person parameterized constructor: " << name_ << "\n";
    }
    
    // Copy constructor
    Person(const Person& other) : name_(other.name_), age_(other.age_) {
        std::cout << "Person copy constructor: " << name_ << "\n";
    }
    
    // Move constructor (C++11)
    Person(Person&& other) noexcept : name_(std::move(other.name_)), age_(other.age_) {
        other.age_ = 0;
        std::cout << "Person move constructor: " << name_ << "\n";
    }
    
    // Copy assignment operator
    Person& operator=(const Person& other) {
        if (this != &other) {
            name_ = other.name_;
            age_ = other.age_;
        }
        std::cout << "Person copy assignment: " << name_ << "\n";
        return *this;
    }
    
    // Move assignment operator (C++11)
    Person& operator=(Person&& other) noexcept {
        if (this != &other) {
            name_ = std::move(other.name_);
            age_ = other.age_;
            other.age_ = 0;
        }
        std::cout << "Person move assignment: " << name_ << "\n";
        return *this;
    }
    
    // Destructor
    virtual ~Person() {
        std::cout << "Person destructor: " << name_ << "\n";
    }
    
    // Getters
    const std::string& name() const { return name_; }
    int age() const { return age_; }
    
    // Setters
    void set_name(const std::string& name) { name_ = name; }
    void set_age(int age) { age_ = age; }
    
    // Virtual function for polymorphism
    virtual void introduce() const {
        std::cout << "Hi, I'm " << name_ << " and I'm " << age_ << " years old.\n";
    }
    
    // Operator overloading
    bool operator<(const Person& other) const {
        return age_ < other.age_;
    }
    
    // Friend function for output
    friend std::ostream& operator<<(std::ostream& os, const Person& person) {
        os << person.name_ << " (" << person.age_ << ")";
        return os;
    }
};

// Inheritance example
class Student : public Person {
private:
    std::string school_;
    double gpa_;
    
public:
    Student(const std::string& name, int age, const std::string& school, double gpa)
        : Person(name, age), school_(school), gpa_(gpa) {
        std::cout << "Student constructor\n";
    }
    
    // Override virtual function
    void introduce() const override {
        std::cout << "Hi, I'm " << name() << ", a student at " << school_ 
                  << " with GPA " << gpa_ << ".\n";
    }
    
    // Additional methods
    const std::string& school() const { return school_; }
    double gpa() const { return gpa_; }
    void set_gpa(double gpa) { gpa_ = gpa; }
};

// Abstract base class
class Shape {
protected:
    std::string color_;
    
public:
    explicit Shape(const std::string& color) : color_(color) {}
    virtual ~Shape() = default;
    
    // Pure virtual functions
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual void draw() const = 0;
    
    // Concrete method
    const std::string& color() const { return color_; }
};

class Circle : public Shape {
private:
    double radius_;
    
public:
    Circle(const std::string& color, double radius) 
        : Shape(color), radius_(radius) {}
    
    double area() const override {
        return 3.14159 * radius_ * radius_;
    }
    
    double perimeter() const override {
        return 2 * 3.14159 * radius_;
    }
    
    void draw() const override {
        std::cout << "Drawing a " << color_ << " circle with radius " << radius_ << "\n";
    }
    
    double radius() const { return radius_; }
};

class Rectangle : public Shape {
private:
    double width_, height_;
    
public:
    Rectangle(const std::string& color, double width, double height)
        : Shape(color), width_(width), height_(height) {}
    
    double area() const override {
        return width_ * height_;
    }
    
    double perimeter() const override {
        return 2 * (width_ + height_);
    }
    
    void draw() const override {
        std::cout << "Drawing a " << color_ << " rectangle " 
                  << width_ << "x" << height_ << "\n";
    }
    
    double width() const { return width_; }
    double height() const { return height_; }
};

// Template class
template<typename T>
class Container {
private:
    std::vector<T> data_;
    
public:
    void add(const T& item) {
        data_.push_back(item);
    }
    
    void add(T&& item) {
        data_.push_back(std::move(item));
    }
    
    const T& get(size_t index) const {
        return data_.at(index);
    }
    
    size_t size() const {
        return data_.size();
    }
    
    // Iterator support
    auto begin() { return data_.begin(); }
    auto end() { return data_.end(); }
    auto begin() const { return data_.begin(); }
    auto end() const { return data_.end(); }
    
    // Template member function
    template<typename Predicate>
    auto find_if(Predicate pred) const {
        return std::find_if(data_.begin(), data_.end(), pred);
    }
};

// RAII class example
class FileHandler {
private:
    std::string filename_;
    bool is_open_;
    
public:
    explicit FileHandler(const std::string& filename) 
        : filename_(filename), is_open_(false) {
        // Simulate file opening
        is_open_ = true;
        std::cout << "File opened: " << filename_ << "\n";
    }
    
    // Delete copy constructor and assignment (move-only)
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;
    
    // Move constructor
    FileHandler(FileHandler&& other) noexcept 
        : filename_(std::move(other.filename_)), is_open_(other.is_open_) {
        other.is_open_ = false;
        std::cout << "File handler moved\n";
    }
    
    // Move assignment
    FileHandler& operator=(FileHandler&& other) noexcept {
        if (this != &other) {
            if (is_open_) {
                std::cout << "File closed: " << filename_ << "\n";
            }
            filename_ = std::move(other.filename_);
            is_open_ = other.is_open_;
            other.is_open_ = false;
        }
        return *this;
    }
    
    ~FileHandler() {
        if (is_open_) {
            std::cout << "File closed: " << filename_ << "\n";
        }
    }
    
    bool is_open() const { return is_open_; }
    const std::string& filename() const { return filename_; }
};

// Static members and methods
class Counter {
private:
    static int count_;
    int id_;
    
public:
    Counter() : id_(++count_) {
        std::cout << "Counter created with ID: " << id_ << "\n";
    }
    
    static int get_count() {
        return count_;
    }
    
    int id() const { return id_; }
};

// Initialize static member
int Counter::count_ = 0;

// Friend class example
class Engine;

class Car {
private:
    std::string model_;
    Engine* engine_;
    
public:
    Car(const std::string& model);
    ~Car();
    
    void start();
    void stop();
    
    friend class Mechanic; // Mechanic can access private members
};

class Engine {
private:
    bool running_;
    int horsepower_;
    
public:
    Engine(int hp) : running_(false), horsepower_(hp) {}
    
    void start() { 
        running_ = true; 
        std::cout << "Engine started (" << horsepower_ << " HP)\n";
    }
    
    void stop() { 
        running_ = false; 
        std::cout << "Engine stopped\n";
    }
    
    bool is_running() const { return running_; }
    int horsepower() const { return horsepower_; }
};

Car::Car(const std::string& model) : model_(model) {
    engine_ = new Engine(200); // Not ideal, just for demonstration
    std::cout << "Car created: " << model_ << "\n";
}

Car::~Car() {
    delete engine_;
    std::cout << "Car destroyed: " << model_ << "\n";
}

void Car::start() {
    engine_->start();
    std::cout << model_ << " is ready to drive\n";
}

void Car::stop() {
    engine_->stop();
    std::cout << model_ << " has stopped\n";
}

class Mechanic {
public:
    void inspect_car(const Car& car) {
        std::cout << "Inspecting " << car.model_ << "\n";
        std::cout << "Engine HP: " << car.engine_->horsepower() << "\n";
    }
};

// Demonstration functions
void demonstrate_basic_classes() {
    std::cout << "\n=== Basic Classes ===\n";
    
    Person person1;
    Person person2("Alice", 25);
    Person person3 = person2; // Copy constructor
    Person person4 = Person("Bob", 30); // Direct construction (no move needed)
    
    person1.introduce();
    person2.introduce();
    person3.introduce();
    person4.introduce();
}

void demonstrate_inheritance() {
    std::cout << "\n=== Inheritance and Polymorphism ===\n";
    
    std::vector<std::unique_ptr<Person>> people;
    people.push_back(std::make_unique<Person>("John", 40));
    people.push_back(std::make_unique<Student>("Emma", 20, "MIT", 3.8));
    
    for (const auto& person : people) {
        person->introduce(); // Polymorphic call
    }
}

void demonstrate_abstract_classes() {
    std::cout << "\n=== Abstract Classes ===\n";
    
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>("red", 5.0));
    shapes.push_back(std::make_unique<Rectangle>("blue", 4.0, 6.0));
    
    for (const auto& shape : shapes) {
        shape->draw();
        std::cout << "Area: " << shape->area() << "\n";
        std::cout << "Perimeter: " << shape->perimeter() << "\n\n";
    }
}

void demonstrate_template_classes() {
    std::cout << "\n=== Template Classes ===\n";
    
    Container<int> int_container;
    int_container.add(1);
    int_container.add(2);
    int_container.add(3);
    
    std::cout << "Integer container: ";
    for (const auto& item : int_container) {
        std::cout << item << " ";
    }
    std::cout << "\n";
    
    Container<std::string> string_container;
    string_container.add("Hello");
    string_container.add("World");
    
    std::cout << "String container: ";
    for (const auto& item : string_container) {
        std::cout << item << " ";
    }
    std::cout << "\n";
}

void demonstrate_raii() {
    std::cout << "\n=== RAII (Resource Acquisition Is Initialization) ===\n";
    
    {
        FileHandler file1("data.txt");
        FileHandler file2 = std::move(file1); // Move
        std::cout << "File2 is open: " << file2.is_open() << "\n";
    } // file2 destroyed here, resource cleaned up automatically
    
    std::cout << "Files automatically closed when going out of scope\n";
}

void demonstrate_static_members() {
    std::cout << "\n=== Static Members ===\n";
    
    std::cout << "Initial counter: " << Counter::get_count() << "\n";
    
    Counter c1;
    Counter c2;
    Counter c3;
    
    std::cout << "Final counter: " << Counter::get_count() << "\n";
}

void demonstrate_friends() {
    std::cout << "\n=== Friend Classes ===\n";
    
    Car my_car("Toyota Camry");
    Mechanic mechanic;
    
    my_car.start();
    mechanic.inspect_car(my_car);
    my_car.stop();
}

} // namespace classes

int main() {
    std::cout << "=== C++ Classes and Objects Demonstration ===\n";
    
    classes::demonstrate_basic_classes();
    classes::demonstrate_inheritance();
    classes::demonstrate_abstract_classes();
    classes::demonstrate_template_classes();
    classes::demonstrate_raii();
    classes::demonstrate_static_members();
    classes::demonstrate_friends();
    
    return 0;
}