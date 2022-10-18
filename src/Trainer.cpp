//
// Created by spl211 on 08/11/2021.
//
#include "../include/Trainer.h"



Trainer:: Trainer(int t_capacity):
        capacity(t_capacity),
        open(false),
        customersList(),
        orderList(),
        salary(0)
{

}
//copy constructor
Trainer:: Trainer(Trainer &other):
        capacity(other.capacity),
        open(other.open),
        customersList(),
        orderList(other.orderList),
        salary(other.salary)
{
    for(size_t i = 0; i < other.customersList.size(); i++)
    {
        Customer* customer;
        if (other.customersList[i]->typeToString() == "swt") // TODO check if == works
        {
            customer = new SweatyCustomer(other.customersList[i]->getName(), other.customersList[i]->getId());
        }
        else if (other.customersList[i]->typeToString() == "chp")
        {
            customer = new CheapCustomer(other.customersList[i]->getName(), other.customersList[i]->getId());
        }
        else if (other.customersList[i]->typeToString() == "mcl")
        {
            customer = new HeavyMuscleCustomer(other.customersList[i]->getName(), other.customersList[i]->getId());
        }
        else
        {
            customer = new FullBodyCustomer(other.customersList[i]->getName(), other.customersList[i]->getId());
        }
        this->customersList.push_back(customer);
    }
}

//destructor
Trainer :: ~Trainer()
{
    clear();
}
void Trainer :: clear()
{
    for (size_t i = 0; i < customersList.size(); i++)
    {
        delete customersList[i];
    }
    customersList.clear();
    orderList.clear();
    salary=0;
}
//move constructor
Trainer:: Trainer(Trainer&& other):
        capacity(other.capacity),
        open(other.open),
        customersList(other.customersList),
        orderList(other.orderList),
        salary(other.salary)
{
    other.capacity = 0;
    other.customersList.clear();
    other.orderList.clear();
    other.salary = 0;
}
//copy assignment
Trainer & Trainer:: operator=(const Trainer &aTrainer)
{
    if (this != &aTrainer)
    {
        this->clear();
        this->capacity = aTrainer.capacity;
        this->open = aTrainer.open;
        this->salary = aTrainer.salary;
        for(size_t i = 0; i < aTrainer.customersList.size(); i++)
        {
            Customer* customer;
            if (customersList[i]->typeToString() == "swt") // TODO check if == works
            {
                customer = new SweatyCustomer(aTrainer.customersList[i]->getName(), aTrainer.customersList[i]->getId());
            }
            else if (customersList[i]->typeToString() == "chp")
            {
                customer = new CheapCustomer(aTrainer.customersList[i]->getName(), aTrainer.customersList[i]->getId());
            }
            else if (customersList[i]->typeToString() == "mcl")
            {
                customer = new HeavyMuscleCustomer(aTrainer.customersList[i]->getName(), aTrainer.customersList[i]->getId());
            }
            else
            {
                customer = new FullBodyCustomer(aTrainer.customersList[i]->getName(), aTrainer.customersList[i]->getId());
            }
            this->customersList.push_back(customer);
        }
        this->orderList = std::vector<OrderPair>(aTrainer.orderList);
        this->salary = aTrainer.salary;
    }
    return *this;
}
//move assignment
Trainer& Trainer:: operator=(Trainer &&other)
{
    if (this != &other)
    {
        this->clear();
        capacity = other.capacity;
        open = other.open;
        customersList = other.customersList;
        for (size_t i = 0; i < other.orderList.size(); i++)
        {
            orderList.push_back(other.orderList[i]);
        }
        salary = other.salary;
        other.capacity = 0;
        other.customersList.clear();
        other.orderList.clear();
        other.salary = 0;
    }
    return *this;
}

int Trainer:: getCapacity() const
{
    return capacity;
}
void Trainer:: addCustomer(Customer* customer)
{
    size_t capacity = getCapacity();
    if (customersList.size() < capacity)
    {
        customersList.push_back(customer);
    }
}
void Trainer:: removeCustomer(int id)
{
    std:: vector<Customer*> newCustomersList;
    for(size_t i = 0; i < customersList.size(); i++)
    {
        if (customersList[i]->getId() != id)
        {
            newCustomersList.push_back(customersList[i]);
        }
    }
    customersList.clear();
    for(size_t i = 0; i < newCustomersList.size(); i++)
    {
        customersList.push_back(newCustomersList[i]);
    }
    newCustomersList.clear();
    std:: vector<OrderPair> newOrderList;
    for(size_t i = 0; i < orderList.size(); i++)
    {
        if (orderList[i].first != id)
        {
            newOrderList.push_back(orderList[i]);
        }
    }
    salary = 0;
    orderList.clear();
    for(size_t i = 0; i < newOrderList.size(); i++)
    {
        orderList.push_back(newOrderList[i]);
        salary = salary + newOrderList[i].second.getPrice();
    }
    newOrderList.clear();
}

Customer* Trainer:: getCustomer(int id)
{
    for (size_t i = 0; i < customersList.size(); i++)
    {
        if (customersList[i] -> getId() == id)
        {
            return customersList[i];
        }
    }
    return nullptr;
}
std::vector<Customer*>& Trainer:: getCustomers()
{
    return customersList;
}
std::vector<OrderPair>& Trainer:: getOrders()
{
    return orderList;
}
void Trainer:: order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options)
{
    Customer* customer = getCustomer(customer_id);
    std:: vector<int> plan = customer->order(workout_options);
    for (size_t i = 0; i < plan.size(); i++)
    {
        Workout pairR = workout_options[plan[i]];
        OrderPair pair = OrderPair(customer->getId(), pairR);
        orderList.push_back(pair);
        salary = salary + pair.second.getPrice();
    }
}
void Trainer:: openTrainer()
{
    open = true;
}
void Trainer:: closeTrainer()
{
    open = false;
    for(size_t i = 0; i < customersList.size(); i++)
    {
        delete customersList[i];
    }
    customersList.clear();
    orderList.clear();
}
int Trainer:: getSalary()
{
    return salary;
}
bool Trainer:: isOpen()
{
    if (open)
        return true;
    else
        return false;
}
//int main()
//{
//    Workout* w0 = new Workout(0,"Yoga", 90, ANAEROBIC);
//    Workout* w1 = new Workout(1,"Pilates", 110, ANAEROBIC);
//    Workout* w2 = new Workout(2,"Spinning", 120, MIXED);
//    Workout* w3 = new Workout(3,"Zumba", 100, CARDIO);
//    Workout* w4 = new Workout(4,"Rope Jumps", 70, CARDIO);
//    Workout* w5 = new Workout(5,"Crossfit", 40, MIXED);
//    std:: vector<Workout> workout_options;
//    workout_options.push_back(*w0);
//    workout_options.push_back(*w1);
//    workout_options.push_back(*w2);
//    workout_options.push_back(*w3);
//    workout_options.push_back(*w4);
//    workout_options.push_back(*w5);
//    SweatyCustomer* swt = new SweatyCustomer("Mor", 0);
//    CheapCustomer* chp = new CheapCustomer("Gev", 1);
//    HeavyMuscleCustomer* mcl = new HeavyMuscleCustomer("Rotem", 2);
//    FullBodyCustomer* fbd = new FullBodyCustomer("Lior", 3);
//    std::vector<int> swt_plan = swt->order(workout_options);
//    std::vector<int> chp_plan = chp->order(workout_options);
//    std::vector<int> fbd_plan = fbd->order(workout_options);
//    Trainer *a = new Trainer(4);
//    Trainer *b = new Trainer(2);
//    Trainer *c = new Trainer(34);
//    Trainer *d = new Trainer(0);
//    std::cout << a->getSalary() << std:: endl;
//    std::cout << a->getCapacity() << std:: endl;
//    std::cout << a->isOpen() << std:: endl;
//    a->openTrainer();
//    std::cout << a->isOpen() << std:: endl;
//    a->addCustomer(swt);
//    a->addCustomer(chp);
//    a->addCustomer(fbd);
//    a->order(0,swt_plan, workout_options);
//    for(int i = 0; i < a->getOrders().size(); i++)
//    {
//        std::cout << a->getOrders()[i].first << a->getOrders()[i].second.getName() << std::endl;
//    }
//    d->addCustomer(chp);
//    std::cout <<d->getCustomers().size()<< std::endl;
//}