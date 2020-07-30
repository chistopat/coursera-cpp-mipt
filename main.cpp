#include <iostream>
#include "map"
#include "cassert"
#include "string"

using namespace std;

// Перечислимый тип для статуса задачи
//enum class TaskStatus {
//    NEW,          // новая
//    IN_PROGRESS,  // в разработке
//    TESTING,      // на тестировании
//    DONE          // завершена
//};
//
//// Объявляем тип-синоним для map<TaskStatus, int>,
//// позволяющего хранить количество задач каждого статуса
//using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
public:
    // Получить статистику по статусам задач конкретного разработчика
    [[nodiscard]] const TasksInfo& GetPersonTasksInfo(const string& person) const {
        return tracker.at(person);
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string& person) {
        tracker[person][TaskStatus::NEW]++;
    }

    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(
            const string& person, int task_count) {
        if (tracker.count(person)==1) {
            return PerformTaskInfo(tracker[person], task_count);
        }
        return make_tuple(TasksInfo(), TasksInfo());
    }
private:
    map<string, TasksInfo> tracker;

    static void EraseEmptyStatus(TasksInfo& tasksInfo) {
        auto temp = tasksInfo;
        for (auto& [k, v] : temp) {
            if (v==0) {
                tasksInfo.erase(k);
            }
        }
    }

    tuple<TasksInfo, TasksInfo> PerformTaskInfo(TasksInfo& taskInfo, int task_count) {
        TasksInfo updated;
        auto untouched = taskInfo;
        untouched.erase(TaskStatus::DONE);
        auto& current = taskInfo;
        for (auto& [status, value] : current) {
            if (status!=TaskStatus::DONE) {
                int minValue = min(task_count, untouched[status]);
                auto nextStatus = TaskStatus(static_cast<int>(status)+1);
                current[status] -= minValue;
                untouched[status] -= minValue;
                updated[nextStatus] += minValue;
                current[nextStatus] += minValue;
                task_count -= minValue;
            }
        }
        EraseEmptyStatus(updated);
        EraseEmptyStatus(untouched);
        EraseEmptyStatus(current);
        return make_tuple(updated, untouched);
    }

};





//void PrintTasksInfo(const TasksInfo& tasks_info) {
//    if (tasks_info.count(TaskStatus::NEW)) {
//        std::cout << "NEW: " << tasks_info.at(TaskStatus::NEW) << " ";
//    }
//    if (tasks_info.count(TaskStatus::IN_PROGRESS)) {
//        std::cout << "IN_PROGRESS: " << tasks_info.at(TaskStatus::IN_PROGRESS) << " ";
//    }
//    if (tasks_info.count(TaskStatus::TESTING)) {
//        std::cout << "TESTING: " << tasks_info.at(TaskStatus::TESTING) << " ";
//    }
//    if (tasks_info.count(TaskStatus::DONE)) {
//        std::cout << "DONE: " << tasks_info.at(TaskStatus::DONE) << " ";
//    }
//}
//
//void PrintTasksInfo(const TasksInfo& updated_tasks, const TasksInfo& untouched_tasks) {
//    std::cout << "Updated: [";
//    PrintTasksInfo(updated_tasks);
//    std::cout << "] ";
//
//    std::cout << "Untouched: [";
//    PrintTasksInfo(untouched_tasks);
//    std::cout << "] ";
//
//    std::cout << std::endl;
//}



//int main() {
//    TeamTasks tasks;
//    TasksInfo updated_tasks;
//    TasksInfo untouched_tasks;
//
//    /* TEST 1 */
//    std::cout << "Alice" << std::endl;
//
//    for (auto i = 0; i < 5; ++i) {
//        tasks.AddNewTask("Alice");
//    }
//    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice", 5);
//    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 5}, {}]
//
//    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice", 5);
//    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"TESTING": 5}, {}]
//
//    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice", 1);
//    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"DONE": 1}, {"TESTING": 4}]
//
//    for (auto i = 0; i < 5; ++i) {
//        tasks.AddNewTask("Alice");
//    }
//    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice",
//                                                                   2);  // [{"IN_PROGRESS": 2}, {"NEW": 3, "TESTING": 4}]
//    PrintTasksInfo(updated_tasks, untouched_tasks);
//
//    PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));  // {"NEW": 3, "IN_PROGRESS": 2, "TESTING": 4, "DONE": 1}
//    std::cout << std::endl;
//
//    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice", 4);
//    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 3, "TESTING": 1}, {"IN_PROGRESS": 1, "TESTING": 4}]
//
//    PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));  // {"IN_PROGRESS": 4, "TESTING": 5, "DONE": 1}
//    std::cout << std::endl;
//
//    /* TEST 2 */
//    std::cout << "\nJack" << std::endl;
//
//    tasks.AddNewTask("Jack");
//
//    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Jack", 1);
//    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 1}, {}]
//
//    tasks.AddNewTask("Jack");
//
//    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Jack", 2);
//    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 1, "TESTING": 1}, {}]
//
//    tasks.AddNewTask("Jack");
//
//    PrintTasksInfo(tasks.GetPersonTasksInfo("Jack"));  // {"NEW": 1, "IN_PROGRESS": 1, "TESTING": 1, "DONE": 0}
//    std::cout << std::endl;
//
//    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Jack", 3);
//    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 1, "TESTING": 1, "DONE": 1}, {}]
//
//    PrintTasksInfo(tasks.GetPersonTasksInfo("Jack"));  // {"IN_PROGRESS": 1, "TESTING": 1, "DONE": 1}
//    std::cout << std::endl;
//
//    /* TEST 3 */
//    std::cout << "\nLisa" << std::endl;
//
//    for (auto i = 0; i < 5; ++i) {
//        tasks.AddNewTask("Lisa");
//    }
//
//    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 5);
//    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 5}, {}]
//
//    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 5);
//    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"TESTING": 5}, {}]
//
//    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 1);
//    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"DONE": 1}, {"TESTING": 4}]
//
//    for (auto i = 0; i < 5; ++i) {
//        tasks.AddNewTask("Lisa");
//    }
//
//    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 2);
//    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 2}, {"NEW": 3, "TESTING": 4}]
//
//    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"NEW": 3, "IN_PROGRESS": 2, "TESTING": 4, "DONE": 1}
//    std::cout << std::endl;
//
//    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 4);
//    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 3, "TESTING": 1}, {"IN_PROGRESS": 1, "TESTING": 4}]
//
//    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"IN_PROGRESS": 4, "TESTING": 5, "DONE": 1}
//    std::cout << std::endl;
//
//    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 5);
//    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"TESTING": 4, "DONE": 1}, {"TESTING": 4}]
//
//    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"TESTING": 8, "DONE": 2}
//    std::cout << std::endl;
//
//    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 10);
//    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"DONE": 8}, {}]
//
//    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"DONE": 10}
//    std::cout << std::endl;
//
//    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 10);
//    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{}, {}]
//
//    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"DONE": 10}
//    std::cout << std::endl;
//
//    tasks.AddNewTask("Lisa");
//
//    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"NEW": 1, "DONE": 10}
//    std::cout << std::endl;
//
//    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 2);
//    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 1}, {}]
//
//    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"IN_PROGRESS": 1, "DONE": 10}
//    std::cout << std::endl;
//
//    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Bob", 3);
//    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{}, {}]
//
//    return 0;
//}

//Alice
//        Updated: [IN_PROGRESS: 5 ] Untouched: []
//Updated: [TESTING: 5 ] Untouched: []
//Updated: [DONE: 1 ] Untouched: [TESTING: 4 ]
//Updated: [IN_PROGRESS: 2 ] Untouched: [NEW: 3 TESTING: 4 ]
//NEW: 3 IN_PROGRESS: 2 TESTING: 4 DONE: 1
//Updated: [IN_PROGRESS: 3 TESTING: 1 ] Untouched: [IN_PROGRESS: 1 TESTING: 4 ]
//IN_PROGRESS: 4 TESTING: 5 DONE: 1
//
//Jack
//        Updated: [IN_PROGRESS: 1 ] Untouched: []
//Updated: [IN_PROGRESS: 1 TESTING: 1 ] Untouched: []
//NEW: 1 IN_PROGRESS: 1 TESTING: 1
//Updated: [IN_PROGRESS: 1 TESTING: 1 DONE: 1 ] Untouched: []
//IN_PROGRESS: 1 TESTING: 1 DONE: 1
//
//Lisa
//        Updated: [IN_PROGRESS: 5 ] Untouched: []
//Updated: [TESTING: 5 ] Untouched: []
//Updated: [DONE: 1 ] Untouched: [TESTING: 4 ]
//Updated: [IN_PROGRESS: 2 ] Untouched: [NEW: 3 TESTING: 4 ]
//NEW: 3 IN_PROGRESS: 2 TESTING: 4 DONE: 1
//Updated: [IN_PROGRESS: 3 TESTING: 1 ] Untouched: [IN_PROGRESS: 1 TESTING: 4 ]
//IN_PROGRESS: 4 TESTING: 5 DONE: 1
//Updated: [TESTING: 4 DONE: 1 ] Untouched: [TESTING: 4 ]
//TESTING: 8 DONE: 2
//Updated: [DONE: 8 ] Untouched: []
//DONE: 10
//Updated: [] Untouched: []
//DONE: 10
//NEW: 1 DONE: 10
//Updated: [IN_PROGRESS: 1 ] Untouched: []
//IN_PROGRESS: 1 DONE: 10
//Updated: [] Untouched: []