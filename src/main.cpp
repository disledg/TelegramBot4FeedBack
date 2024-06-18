#include <tgbot/tgbot.h>
#include <string>
#include <iostream>
#include <unordered_map>
#include <map>

const std::string BOT_TOKEN = "7377075467:AAFGc1EjScNueDYA1p-93s5PkV18TzcbaUM";
const std::int64_t GROUP_CHAT_ID = -4227593901;  // ID вашей группы


enum class BugSeverity {
    None,
    Low,
    Medium,
    High
};


enum class BugCriticality {
    None,
    Blocker,
    Critical,
    Major,
    Minor,
    Trivial
};       
    

struct UserState {
    bool isSeverityEnter = false;
    bool isDesc = false;
    BugSeverity severity = BugSeverity::None;
    BugCriticality criticality = BugCriticality::None;
    std::string description;
};

std::map<int, UserState> userStates;
// Criticality of bug (Custom Keyboard)
void hundleBugCriticality(TgBot::Bot& bot, TgBot::Message::Ptr message){
    userStates[message->from->id].criticality = BugCriticality::None;

    TgBot::ReplyKeyboardMarkup::Ptr keyboard(new TgBot::ReplyKeyboardMarkup);

    std::vector<TgBot::KeyboardButton::Ptr> row1;
    TgBot::KeyboardButton::Ptr Blocker(new TgBot::KeyboardButton);
    Blocker->text = "Blocker";
    row1.push_back(Blocker);
    Blocker->requestContact = false;
    Blocker->requestLocation = false;

    std::vector<TgBot::KeyboardButton::Ptr> row2;
    TgBot::KeyboardButton::Ptr Critical(new TgBot::KeyboardButton);
    Critical->text = "Critical";
    row2.push_back(Critical);
    Critical->requestContact = false;
    Critical->requestLocation = false;

    std::vector<TgBot::KeyboardButton::Ptr> row3;
    TgBot::KeyboardButton::Ptr Minor(new TgBot::KeyboardButton);
    Minor->text = "Minor";
    row3.push_back(Minor);
    Minor->requestContact = false;
    Minor->requestLocation = false;

    keyboard->keyboard.push_back(row1);
    keyboard->keyboard.push_back(row2);
    keyboard->keyboard.push_back(row3);
    keyboard->resizeKeyboard = true;  // Разрешить изменение размера клавиатуры пользователем
    keyboard->oneTimeKeyboard = true; // Клавиатура будет скрыта после нажатия на кнопку

    bot.getApi().sendMessage(message->chat->id, "Выберите критичность бага:", 0, 0, keyboard);
}
// Criticality of bug (Custom Keyboard)
void handleBugReport(TgBot::Bot& bot, TgBot::Message::Ptr message) {
    userStates[message->from->id].isSeverityEnter = true;
    userStates[message->from->id].severity = BugSeverity::None; // Сбрасываем предыдущую сложность

    TgBot::ReplyKeyboardMarkup::Ptr keyboard(new TgBot::ReplyKeyboardMarkup);

    std::vector<TgBot::KeyboardButton::Ptr> row1;
    TgBot::KeyboardButton::Ptr button1(new TgBot::KeyboardButton);
    button1->text = "Low";
    row1.push_back(button1);
    button1->requestContact = false;
    button1->requestLocation = false;

    std::vector<TgBot::KeyboardButton::Ptr> row2;
    TgBot::KeyboardButton::Ptr button2(new TgBot::KeyboardButton);
    button2->text = "Medium";
    row2.push_back(button2);
    button2->requestContact = false;
    button2->requestLocation = false;

    std::vector<TgBot::KeyboardButton::Ptr> row3;
    TgBot::KeyboardButton::Ptr button3(new TgBot::KeyboardButton);
    button3->text = "High";
    row3.push_back(button3);
    button3->requestContact = false;
    button3->requestLocation = false;

    keyboard->keyboard.push_back(row1);
    keyboard->keyboard.push_back(row2);
    keyboard->keyboard.push_back(row3);

    keyboard->resizeKeyboard = true;  
    keyboard->oneTimeKeyboard = true; 
    bot.getApi().sendMessage(message->chat->id, "Выберите приоритет:", 0, 0, keyboard);
}


int main() {
    TgBot::Bot bot(BOT_TOKEN);

    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Введите /bug_report, для создания баг репорта.");
    });
    //Description && send to group
    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message){
        int UserId = message->from->id;
        if(userStates[UserId].isDesc){
        userStates[UserId].description = message->text;
        std::string reportText = "Bug report:\nПриоритет: ";
        switch (userStates[UserId].severity) {
            case BugSeverity::Low:
                reportText += "Low🟡\n";
                break;
            case BugSeverity::Medium:
                reportText += "Medium⚡️\n";
                break;
            case BugSeverity::High:
                reportText += "High🔥\n";
                break;
            default:
                reportText += "None\n"; 
                break;
        }
        reportText += "Критичность:";
        switch (userStates[UserId].criticality) {
            case BugCriticality::Blocker:
                reportText += "Blocker\n";
                break;
            case BugCriticality::Critical:
                reportText += "Critical\n";
                break;
            case BugCriticality::Minor:
                reportText += "Minor\n";
                break;
            default:
                reportText += "None\n"; 
                break;
        }
        reportText += "Описание:" + userStates[message->from->id].description;
        bot.getApi().sendMessage(GROUP_CHAT_ID, reportText);

        userStates[UserId].isSeverityEnter = false;
        userStates[UserId].isDesc = false;
        userStates[UserId].severity = BugSeverity::None; 
        userStates[UserId].criticality = BugCriticality::None;
        }
    });
    // Command /bug_report
    bot.getEvents().onCommand("bug_report",[&bot](TgBot::Message::Ptr message){
        handleBugReport(bot,message);
    });
    bot.getEvents().onNonCommandMessage([&bot](TgBot::Message::Ptr message){
        int UserId = message->from->id;
        if(userStates[UserId].isSeverityEnter){
        // Severity.start()
            if(message->text == "Low"){
            userStates[UserId].severity = BugSeverity::Low;
            hundleBugCriticality(bot,message);
            }
            else if(message->text == "Medium"){
            userStates[UserId].severity = BugSeverity::Medium;
            hundleBugCriticality(bot,message);
            }
            else if(message->text == "High"){
            userStates[UserId].severity = BugSeverity::High;
            hundleBugCriticality(bot,message);
            }
        // Severity.end()
        // Criticality.start()
            else if(message->text == "Blocker"){
                userStates[UserId].criticality = BugCriticality::Blocker;
                userStates[UserId].isDesc = true;
                userStates[UserId].isSeverityEnter = false;
                bot.getApi().sendMessage(message->chat->id, "Введите описание ошибки:");
            }
            else if(message->text == "Critical"){
                userStates[UserId].criticality = BugCriticality::Critical;
                userStates[UserId].isDesc = true;
                userStates[UserId].isSeverityEnter = false;
                bot.getApi().sendMessage(message->chat->id, "Введите описание ошибки:");
            }
            else if(message->text == "Trivial"){
                userStates[UserId].criticality = BugCriticality::Trivial;
                userStates[UserId].isDesc = true;
                userStates[UserId].isSeverityEnter = false;
                bot.getApi().sendMessage(message->chat->id, "Введите описание ошибки:");
            }
        //Criticality.end()
        }
       
    });

    try {
        std::cout << "Bot username: " << bot.getApi().getMe()->username << std::endl;
        bot.getApi().deleteWebhook();
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        std::cerr << "Telegram Error: " << e.what() << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
