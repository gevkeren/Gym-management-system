//
// Created by spl211 on 17/11/2021.
//
#include "../include/Action.h"


//class BaseAction
//constructor
BaseAction:: BaseAction():
        errorMsg(),
        status()
{

}
//public
ActionStatus BaseAction:: getStatus() const
{
    return status;
}
//protected
void BaseAction:: complete()
{
    status = COMPLETED;
}
void BaseAction:: error(std::string errorMsg)
{
    status = ERROR;
    std:: cout << "ERROR: " << errorMsg << std::endl;
}
std::string BaseAction:: getErrorMsg() const
{
    return errorMsg;
}

//class OpenTrainer
//public
OpenTrainer:: OpenTrainer(int id, std::vector<Customer *> &customersList):
    BaseAction(),
    trainerId(id),
    customers(customersList)
{

}
OpenTrainer:: ~OpenTrainer() = default;

void OpenTrainer:: act(Studio &studio)
{
    Trainer * trainer = studio.getTrainer(trainerId);
    if ((trainer == nullptr) || (trainer->isOpen()))
        error("Workout session does not exist or is already open");
    else
    {
        trainer -> openTrainer();
        for (size_t i = 0; (i < customers.size()) && (trainer->getCapacity() > 0); i++)
        {
            trainer->addCustomer(customers[i]);
        }
        complete();
    }
    toLog = "open ";
    toLog.append(std::to_string(trainerId));
    for (size_t i = 0; i < customers.size(); i++)
    {
        toLog.append(" " + customers[i]->getName());
        toLog.append(",");
        toLog.append(customers[i]->typeToString());
    }
    this->customers.clear();
    if (getStatus() == COMPLETED)
        toLog.append(" Completed");
    else
        toLog.append(" ERROR: Workout session does not exist or is already open");
}
std::string OpenTrainer:: toString() const
{
    return toLog;
}
OpenTrainer* OpenTrainer:: clone()
{
    OpenTrainer *clone =  new OpenTrainer(trainerId, customers);
    clone->toLog = this->toLog;
    return clone;
}
//class Order
//public
Order:: Order(int id):
    trainerId(id)
{

}
Order:: ~Order() = default;

void Order:: act(Studio &studio)
{
    Trainer *t = studio.getTrainer(trainerId);
    std:: vector<int> wo_ids;
    std:: vector<Workout> wo_opt = studio.getWorkoutOptions();
    if ((t == nullptr) || (!t->isOpen()))
    {
        error("Trainer does not exist or is not open");
    }
    else
    {
        for (size_t i = 0; i < t->getCustomers().size(); i++)
        {
            //wo_ids = t->getCustomer(i)->order(wo_opt);
            int id = t->getCustomers()[i]->getId();
            t->order(id, wo_ids, wo_opt);
        }
        for(size_t j = 0; j < t->getOrders().size(); j++)
        {
            std::cout << t->getCustomer(t->getOrders()[j].first)->getName() << " Is Doing " << t->getOrders()[j].second.getName() << std::endl;
        }
        complete();
    }
}
std::string Order:: toString() const
{
    std::string orderCommand = "order ";
    orderCommand.append(std::to_string(trainerId));
    if (getStatus() == COMPLETED)
        orderCommand.append(" Completed");
    else
        orderCommand.append(" ERROR: Trainer does not exist or is not open");
    return orderCommand;
}
Order* Order:: clone()
{
    return new Order(trainerId);
}

//class MoveCustomer
//public
MoveCustomer:: MoveCustomer(int src, int dst, int customerId):
    srcTrainer(src),
    dstTrainer(dst),
    id(customerId)
{

}
MoveCustomer:: ~MoveCustomer() = default;

void MoveCustomer:: act(Studio &studio)
{
    Trainer *src = studio.getTrainer(srcTrainer);
    Trainer *dst = studio.getTrainer(dstTrainer);
    size_t dstCapacity = dst->getCapacity();
    if((!src->isOpen()) || (!dst->isOpen()) || (src->getCustomer(id) == nullptr) || (dstCapacity == dst->getCustomers().size()))
    {
        error("Cannot move customer");
    }
    else
    {
        std:: vector<Workout> wo_options = studio.getWorkoutOptions();
        std:: vector<int> wo_ids = src->getCustomer(id)->order(wo_options);
        dst->addCustomer(src->getCustomer(id));
        dst->order(id, wo_ids, wo_options);
        src->removeCustomer(id);
        //finished moving the customer, now checking if closing the origin trainer is necessary
        if (src->getCustomers().empty())
        {
            Close* closeAction = new Close(srcTrainer);
            closeAction->act(studio);
            delete closeAction;
        }
        complete();
    }
}
std::string MoveCustomer:: toString() const
{
    std::string moveCustomerCommand = "move ";
    moveCustomerCommand.append(std::to_string(srcTrainer));
    moveCustomerCommand.append(" ");
    moveCustomerCommand.append(std::to_string(dstTrainer));
    moveCustomerCommand.append(" ");
    moveCustomerCommand.append(std::to_string(id));
    if (getStatus() == COMPLETED)
        moveCustomerCommand.append(" Completed");
    else
        moveCustomerCommand.append(" ERROR: Cannot move customer");
    return moveCustomerCommand;
}
MoveCustomer* MoveCustomer:: clone()
{
    return new MoveCustomer(srcTrainer, dstTrainer, id);
}

//class Close
//public
Close:: Close(int id):
        trainerId(id)
{

}
Close:: ~Close() = default;

void Close:: act(Studio &studio)
{
    Trainer *trainer = studio.getTrainer(trainerId);
    if ((trainer == nullptr) || (!trainer->isOpen()))
    {
        error("Trainer does not exist or is not open");
    }
    else
    {
        trainer->closeTrainer();
        std::cout << "Trainer " << trainerId << " closed. Salary " << trainer->getSalary() << "NIS" << std::endl;
        complete();
    }


}
std::string Close:: toString() const
{
    std::string closeCommand = "close ";
    closeCommand.append(std::to_string(trainerId));
    if (getStatus() == COMPLETED)
        closeCommand.append(" Completed");
    else
        closeCommand.append(" ERROR: Trainer does not exist or is not open");
    return closeCommand;
}
Close* Close:: clone()
{
    return new Close(trainerId);
}


//class CloseAll
//public
CloseAll:: CloseAll()
{

}
CloseAll:: ~CloseAll() = default;
void CloseAll:: act(Studio &studio)
{
    std::vector<Trainer*> trainersList = studio.getTrainers();
    for(size_t i = 0; i < trainersList.size(); i++)
    {
        std::cout << "Trainer " << i << " closed. Salary " << trainersList[i]->getSalary() << "NIS" << std:: endl;
        trainersList[i]->closeTrainer();
    }
    complete();
}
std::string CloseAll:: toString() const
{
    std::string closeAllCommand = "closeall";
    closeAllCommand.append(" Completed");
    return closeAllCommand;
}
CloseAll* CloseAll:: clone()
{
    return new CloseAll();
}


//class PrintWorkoutOptions
//public
PrintWorkoutOptions:: PrintWorkoutOptions()
{

}
PrintWorkoutOptions:: ~PrintWorkoutOptions() = default;
void PrintWorkoutOptions:: act(Studio &studio)
{
    //name, type, price
    std:: vector<Workout> workout_options = studio.getWorkoutOptions();
    for (size_t i = 0; i < workout_options.size(); i++)
    {
        std::string type;
        if(workout_options[i].getType() == ANAEROBIC)
            type = "Anaerobic";
        if(workout_options[i].getType() == MIXED)
            type = "Mixed";
        if(workout_options[i].getType() == CARDIO)
            type = "Cardio";
        std::cout<<workout_options[i].getName()<<", "<<type<<", "<<workout_options[i].getPrice()<<std::endl;
    }
    complete();
}
std::string PrintWorkoutOptions:: toString() const
{
    std::string printWorkoutOptionsCommand = "print workout options Completed";
    return printWorkoutOptionsCommand;
}
PrintWorkoutOptions* PrintWorkoutOptions:: clone()
{
    return new PrintWorkoutOptions();
}


//class PrintTrainerStatus
//public
PrintTrainerStatus:: PrintTrainerStatus(int id):
        trainerId(id)
{

}
PrintTrainerStatus:: ~PrintTrainerStatus() = default;
void PrintTrainerStatus:: act(Studio &studio)
{
    //status, customersList, orderslist
    Trainer *t = studio.getTrainer(trainerId);
    if (t->isOpen())
    {
        std:: cout << "Trainer " << trainerId << " status: open" << std::endl;
        std:: cout << "Customers:" << std:: endl;
        for(size_t i = 0; i < t->getCustomers().size(); i++)
        {
            std:: cout << t->getCustomers()[i]->getId() << " " << t->getCustomers()[i]->getName() << std::endl;
        }
        std::cout<<"Orders:"<<std:: endl;
        for(size_t i = 0; i < t->getOrders().size(); i++)
        {
            std::cout<<t->getOrders()[i].second.getName()<<" "<<t->getOrders()[i].second.getPrice()<< "NIS "<<t->getOrders()[i].first<<std::endl;
        }
        std::cout << "Current Trainer's Salary: " << t->getSalary() << "NIS" << std::endl;
    }
    else
    {
        std:: cout << "Trainer " << trainerId << " status: closed" << std::endl;
    }
    complete();

}
std::string PrintTrainerStatus:: toString() const
{
    std:: string printTrainerStatusCommand = "status ";
    printTrainerStatusCommand.append(std::to_string(trainerId));
    printTrainerStatusCommand.append(" Completed");
    return printTrainerStatusCommand;
}
PrintTrainerStatus* PrintTrainerStatus:: clone()
{
    return new PrintTrainerStatus(trainerId);
}


//class PrintActionsLog
//public
PrintActionsLog:: PrintActionsLog()
{

}
PrintActionsLog:: ~PrintActionsLog() = default;
void PrintActionsLog:: act(Studio &studio)
{
    std::vector<BaseAction*> actions = studio.getActionsLog();
    for(size_t i = 0; i < actions.size(); i++)
    {
        std::cout << actions[i]->toString() << std::endl;
    }
    complete();
}
std::string PrintActionsLog:: toString() const
{
    std::string printActionsLogCommand = "print actions log Completed";
    return printActionsLogCommand;
}
PrintActionsLog* PrintActionsLog:: clone()
{
    return new PrintActionsLog();
}


//class BackupStudio
//public

//class BackupStudio
//public
BackupStudio:: BackupStudio()
{

}
BackupStudio:: ~BackupStudio() = default;
extern Studio* backup;
void BackupStudio:: act(Studio &studio)
{
    Studio *temp = backup;
    backup = new Studio(studio);
    delete temp;
    complete();
}
std::string BackupStudio:: toString() const
{
    std:: string backupStudioAction = "backup Completed";
    return backupStudioAction;
}
BackupStudio* BackupStudio:: clone()
{
    return new BackupStudio();
}


//class RestoreStudio
//public
RestoreStudio:: RestoreStudio()
{

}
RestoreStudio:: ~RestoreStudio() = default;
void RestoreStudio:: act(Studio &studio)
{
    if(backup != nullptr)
    {
        studio = *backup;
        complete();
    }
    else
        error("no backup available");
}
std::string RestoreStudio:: toString() const
{
    std:: string restoreStudioAction = "restore";
    if (getStatus() == COMPLETED)
        restoreStudioAction.append(" Completed");
    else
        restoreStudioAction.append(" ERROR: no backup available");
    return restoreStudioAction;
}
RestoreStudio* RestoreStudio:: clone()
{
    return new RestoreStudio();
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
//    a->addCustomer(swt);
//    a->addCustomer(chp);
//    a->addCustomer(fbd);
//    Trainer *b = new Trainer(2);
//    Trainer *c = new Trainer(34);
//    Trainer *d = new Trainer(0);
//    OpenTrainer* open = new OpenTrainer(0, a->getCustomers());
//    for(int i = 0; i < a->getOrders().size(); i++){
//        std::cout << a->getOrders()[i].first << a->getOrders()[i].second.getName() << std::endl;
//    }

//}