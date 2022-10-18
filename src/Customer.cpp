//
// Created by spl211 on 08/11/2021.
//
# include "../include/Customer.h"


//Customer
Customer:: Customer(std::string c_name, int c_id):
        name(c_name),
        id(c_id)
{

}
std::string Customer:: getName() const
{
    return name;
}
int Customer:: getId() const
{
    return id;
}

//Sweaty Customer - swt
SweatyCustomer:: SweatyCustomer(std::string name, int id):
        Customer(name, id)
{

}
SweatyCustomer:: ~SweatyCustomer() = default;

std::vector<int> SweatyCustomer:: order(const std::vector<Workout> &workout_options)
{
    std:: vector <int> swt_plan;
    for (size_t i = 0; i < workout_options.size(); i++)
    {
        if (workout_options[i].getType() == CARDIO)
        {
            swt_plan.push_back(workout_options[i].getId());
        }
    }
    return swt_plan;
}
std::string SweatyCustomer:: toString() const
{
    std:: string toStr = "";
    toStr.append(std::to_string(getId()));
    toStr.append(" ");
    toStr.append(getName());
    return toStr;
}
std:: string SweatyCustomer:: typeToString()
{
    return "swt";
}

//Cheap Customer - CHP
CheapCustomer:: CheapCustomer(std::string name, int id):
        Customer(name, id)
{

}
CheapCustomer:: ~CheapCustomer() = default;

std::vector<int> CheapCustomer:: order(const std::vector<Workout> &workout_options)
{
    std:: vector <int> chp_plan;
    chp_plan.push_back(workout_options[0].getId());
    int minPrice = workout_options[0].getPrice();
    for (size_t i = 1; i < workout_options.size(); i++)
    {
        if ( (workout_options[i].getPrice() < minPrice) ||
             ((workout_options[i].getPrice() == minPrice) && (workout_options[i].getId() < chp_plan[0])) )
        {
            chp_plan[0] = workout_options[i].getId();
            minPrice = workout_options[i].getPrice();
        }
    }
    return chp_plan;
}
std::string CheapCustomer:: toString() const
{
    std:: string toStr = "";
    toStr.append(std::to_string(getId()));
    toStr.append(" ");
    toStr.append(getName());
    return toStr;
}
std:: string CheapCustomer:: typeToString()
{
    return "chp";
}

//HeavyMuscle Customer - MCL
HeavyMuscleCustomer:: HeavyMuscleCustomer(std::string name, int id):
    Customer(name, id)
{

}
HeavyMuscleCustomer:: ~HeavyMuscleCustomer() = default;

std::vector<int> HeavyMuscleCustomer:: order(const std::vector<Workout> &workout_options)
{
    std:: vector <int> mcl_plan;
    std:: vector<std::pair<int,int>> mclPairPlan;
    for (size_t i = 0; i < workout_options.size(); i++)
    {
        if (workout_options[i].getType() == ANAEROBIC)
        {
            std::pair<int,int> pair(workout_options[i].getPrice(), workout_options[i].getId());
            mclPairPlan.push_back(pair);
        }
    }
    std::sort(mclPairPlan.begin(), mclPairPlan.end(), mclComparator);//now the mclPairPlan vector is sorted
    for (size_t i = 0; i < mclPairPlan.size(); i++)
    {
        mcl_plan.push_back(mclPairPlan[i].second);
    }
    return mcl_plan;
}
bool mclComparator(std::pair<int, int> &pair1,std::pair<int, int> &pair2)
{
    if (pair1.first != pair2.first)
    {
        return pair1.first > pair2.first;
    }
    else
    {
        return pair1.second < pair2.second;
    }
}
std::string HeavyMuscleCustomer:: toString() const
{
    std:: string toStr = "";
    toStr.append(std::to_string(getId()));
    toStr.append(" ");
    toStr.append(getName());
    return toStr;
}
std:: string HeavyMuscleCustomer:: typeToString()
{
    return "mcl";
}

//Full Body Customer - FBD
FullBodyCustomer:: FullBodyCustomer(std::string name, int id):
        Customer(name, id)
{

}
FullBodyCustomer:: ~FullBodyCustomer() = default;

std::vector<int> FullBodyCustomer:: order(const std::vector<Workout> &workout_options)
//cheapest cardio --> most expensive mix --> cheapest anaerobic
{
    std:: vector <Workout> cardio_v;
    std:: vector <Workout> mixed_v;
    std:: vector <Workout> anaerobic_v;
    std:: vector <int> fbd_plan;
    for (size_t i = 0; i < workout_options.size(); i++)
    {
        if (workout_options[i].getType() == ANAEROBIC)
        {
            anaerobic_v.push_back(workout_options[i]);
        }
        else if (workout_options[i].getType() == CARDIO)
        {
            cardio_v.push_back(workout_options[i]);
        }
        else
        {
            mixed_v.push_back(workout_options[i]);
        }
    }

    //pushing the cheapest cardio
    int minPriceC = cardio_v[0].getPrice();
    int IDC = cardio_v[0].getId();
    for (size_t i = 1; i < cardio_v.size(); i++)
    {
        if ( (cardio_v[i].getPrice() < minPriceC) ||
             ((cardio_v[i].getPrice() == minPriceC) && (cardio_v[i].getId() < IDC)) )
        {
            minPriceC = cardio_v[i].getPrice();
            IDC = cardio_v[i].getId();
        }
    }
    fbd_plan.push_back(IDC);

    //pushing the most expensive mixed
    int maxPriceM = mixed_v[0].getPrice();
    int IDM = mixed_v[0].getId();
    for (size_t i = 1; i < mixed_v.size(); i++)
    {
        if ( (mixed_v[i].getPrice() > maxPriceM) ||
             ((mixed_v[i].getPrice() == maxPriceM) && (mixed_v[i].getId() < IDM)) )
        {
            maxPriceM = mixed_v[i].getPrice();
            IDM = mixed_v[i].getId();
        }
    }
    fbd_plan.push_back(IDM);

    //pushing the cheapest anaerobic
    int minPriceA = anaerobic_v[0].getPrice();
    int IDA = anaerobic_v[0].getId();
    for (size_t i = 1; i < anaerobic_v.size(); i++)
    {
        if ( (anaerobic_v[i].getPrice() < minPriceA) ||
             ((anaerobic_v[i].getPrice() == minPriceA) && (anaerobic_v[i].getId() < IDA)) )
        {
            minPriceA = anaerobic_v[i].getPrice();
            IDA = anaerobic_v[i].getId();
        }
    }
    fbd_plan.push_back(IDA);
    return fbd_plan;
}
std::string FullBodyCustomer:: toString() const
{
    std:: string toStr = "";
    toStr.append(std::to_string(getId()));
    toStr.append(" ");
    toStr.append(getName());
    return toStr;
}
std:: string FullBodyCustomer:: typeToString()
{
    return "fbd";
}
//int main()
//{
//    Workout w0 = Workout(9,"Yoga", 90, ANAEROBIC);
//    Workout w1 = Workout(6,"Pilates", 110, ANAEROBIC);
//    Workout w1a = Workout(10,"a", 110, ANAEROBIC);
//    Workout w1b = Workout(0,"b", 110, ANAEROBIC);
//    Workout w1c = Workout(1,"c", 90, ANAEROBIC);
//    Workout w1d = Workout(20,"d", 110, ANAEROBIC);
//    Workout w2 = Workout(2,"Spinning", 120, MIXED);
//    Workout w3 = Workout(3,"Zumba", 100, CARDIO);
//    Workout w4 = Workout(4,"Rope Jumps", 70, CARDIO);
//    Workout w5 = Workout(5,"Crossfit", 40, MIXED);
//    std:: vector<Workout> workout_options;
//    workout_options.push_back(w0);
//    workout_options.push_back(w1);
//    workout_options.push_back(w2);
//    workout_options.push_back(w3);
//    workout_options.push_back(w4);
//    workout_options.push_back(w5);
//    workout_options.push_back(w1a);
//    workout_options.push_back(w1b);
//    workout_options.push_back(w1c);
//    workout_options.push_back(w1d);
//    SweatyCustomer* swt = new SweatyCustomer("Mor", 0);
//    CheapCustomer* chp = new CheapCustomer("Gev", 1);
//    HeavyMuscleCustomer* mcl = new HeavyMuscleCustomer("Rotem", 2);
//    FullBodyCustomer* fbd = new FullBodyCustomer("Lior", 3);
//    std::vector<int> swt_plan = swt->order(workout_options);
//    for (int i = 0; i < swt_plan.size(); i++)
//    {
//        std::cout << swt_plan[i];//3,4
//    }
//    std::cout <<std:: endl;
//    std::vector<int> chp_plan = chp->order(workout_options);
//    for (int i = 0; i < chp_plan.size(); i++)
//    {
//        std::cout << chp_plan[i];//5
//    }
//    std::cout <<std:: endl;
//    std::vector<int> mcl_plan = mcl->order(workout_options);
//    for (int i = 0; i < mcl_plan.size(); i++)
//    {
//        std::cout << mcl_plan[i];//06102019
//    }
//    std::cout <<std:: endl;
//    std::vector<int> fbd_plan = fbd->order(workout_options);
//    for (int i = 0; i < fbd_plan.size(); i++)
//    {
//        std::cout << fbd_plan[i];//4,2,0
//    }
//    std::cout <<std:: endl;
//    std::cout << swt->toString() << std:: endl;
//    std::cout << chp->toString() << std:: endl;
//    std::cout << mcl->toString() << std:: endl;
//    std::cout << fbd->toString() << std:: endl;
//}