/*
    Files Updated: Application.cpp, CMakeLists.txt, Application.h
    New Files: Logger.h, Logger.cpp
    Changes: added a logging system for ImGui using a logger class
    There are 2 log types: General and Game
    There are 3 log levels: Info, Warning, Error
    Logs can be filtered by level, cleared, saved to file, or copied to clipboard
    The user can also type in the console input box to log custom messages
*/
#include "Application.h"
#include "imgui/imgui.h"
#include "Logger.h"

namespace ClassGame {
        bool log_window_visible = true;
        static char input_buffer[256] = "";  // Buffer for console input
        static int log_filter_level = 2;  // 0=Info, 1=Warning, 2=Error (show all)
        
        void GameStartUp() 
        {
            // Initialize logging system
            Logger& logger = Logger::GetInstance();
            logger.LogInfo("Game started successfully");
            logger.LogGameEvent("Application initialized");
        }

        //
        // game render loop
        // this is called by the main render loop in main.cpp
        //
        void RenderGame() 
        {
            ImGui::DockSpaceOverViewport();
            ImGui::ShowDemoWindow();

            ImGui::Begin("ImGui Log Demo");
            ImGui::LogButtons();

            if (ImGui::Button("Copy \"Hello, world!\" to clipboard"))
            {
                ImGui::LogToClipboard();
                ImGui::LogText("Hello, world!");
                ImGui::LogFinish();
            }
            ImGui::End();
            
            // Render game window with log test buttons
            RenderGameWindow();
            
            // Display the log window if it's visible
            if (IsLogWindowVisible())
            {
                ShowLogWindow();
            }
        }

        bool IsLogWindowVisible() 
        {
            return log_window_visible;
        }

        void ToggleLogWindow() 
        {
            log_window_visible = !log_window_visible;
        }

        void RenderGameWindow()
        {
            ImGui::Begin("Game Control");
            ImGui::Text("Game Log Test Buttons");
            ImGui::Separator();
            
            ImGui::Text("Log Level Tests:");
            
            if (ImGui::Button("Info", ImVec2(100, 0)))
            {
                Logger::GetInstance().LogInfo("Test info message from game");
            }
            ImGui::SameLine();
            if (ImGui::Button("Warning", ImVec2(100, 0)))
            {
                Logger::GetInstance().LogWarning("Test warning message from game");
            }
            ImGui::SameLine();
            if (ImGui::Button("Error", ImVec2(100, 0)))
            {
                Logger::GetInstance().LogError("Test error message from game");
            }
            
            ImGui::Text("Game Event Tests:");
            
            if (ImGui::Button("Game Event", ImVec2(100, 0)))
            {
                Logger::GetInstance().LogGameEvent("Player made a move");
            }
            ImGui::SameLine();
            if (ImGui::Button("Game Warning", ImVec2(100, 0)))
            {
                Logger::GetInstance().LogGameWarning("Invalid move attempted");
            }
            ImGui::SameLine();
            if (ImGui::Button("Game Error", ImVec2(100, 0)))
            {
                Logger::GetInstance().LogGameError("Game state corrupted");
            }
            
            ImGui::Separator();
            if (ImGui::Button("Toggle Log Window", ImVec2(200, 0)))
            {
                ToggleLogWindow();
            }
            
            ImGui::End();
        }

        void ShowLogWindow(bool* p_open /*= nullptr*/) 
        {
            // Use p_open if provided, otherwise use the internal log_window_visible
            bool* visibility_ptr = p_open ? p_open : &log_window_visible;
            
            if (ImGui::Begin("Game Log", visibility_ptr))
            {
                if (ImGui::Button("Options"))
                {
                    ImGui::OpenPopup("options_popup");
                }
                if (ImGui::BeginPopup("options_popup"))
                {
                    ImGui::Text("Log Filter Level:");
                    ImGui::RadioButton("Info (show Info only)", &log_filter_level, 0);
                    ImGui::RadioButton("Warning (show Info + Warning)", &log_filter_level, 1);
                    ImGui::RadioButton("Error (show all)", &log_filter_level, 2);
                    ImGui::EndPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Clear"))
                {
                    // Clear log entries from Logger
                    Logger::GetInstance().ClearLog();
                }
                ImGui::SameLine();
                if (ImGui::Button("Save To File"))
                {
                    // Save current log to a file next to the demo binary
                    Logger::GetInstance().SaveToFile("imgui_log.txt");
                }
                ImGui::SameLine();
                if (ImGui::Button("Copy To Clipboard"))
                {
                    // Copy all entries to system clipboard
                    std::string all = Logger::GetInstance().ExportToString();
                    ImGui::SetClipboardText(all.c_str());
                }
                
                ImGui::Separator();
                
                // Scrolling log display area
                const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
                if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar))
                {
                    // Display log entries from Logger with formatting and colors
                    for (const auto& entry : Logger::GetInstance().GetEntries())
                    {
                        // Filter based on selected log level
                        bool show_entry = false;
                        if (log_filter_level == 0)  // Info only
                        {
                            show_entry = (entry.level == LogLevel::Info || entry.level == LogLevel::GameEvent);
                        }
                        else if (log_filter_level == 1)  // Warning and above
                        {
                            show_entry = (entry.level == LogLevel::Info || entry.level == LogLevel::GameEvent ||
                                        entry.level == LogLevel::Warning || entry.level == LogLevel::GameWarning);
                        }
                        else  // Error (show all)
                        {
                            show_entry = true;
                        }
                        
                        if (!show_entry)
                            continue;
                        
                        // Determine color based on log level
                        ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Default white
                        
                        if (entry.level == LogLevel::Error || entry.level == LogLevel::GameError)
                        {
                            color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); // Red
                        }
                        else if (entry.level == LogLevel::Warning || entry.level == LogLevel::GameWarning)
                        {
                            color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); // Orange
                        }
                        
                        ImGui::PushStyleColor(ImGuiCol_Text, color);
                        char buffer[512];
                        snprintf(buffer, sizeof(buffer), "[%02d:%02d:%02d] %s %s",
                            entry.hour, entry.minute, entry.second,
                            entry.tag.c_str(), entry.message.c_str());
                        ImGui::TextUnformatted(buffer);
                        ImGui::PopStyleColor();
                    }
                    
                    // Auto-scroll to bottom if user was already at bottom
                    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                        ImGui::SetScrollHereY(1.0f);
                }
                ImGui::EndChild();
                
                // Input text box at bottom
                ImGui::Separator();
                bool reclaim_focus = false;
                ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
                if (ImGui::InputText("Input", input_buffer, sizeof(input_buffer), input_flags))
                {
                    // User pressed Enter
                    if (input_buffer[0] != '\0')
                    {
                        Logger::GetInstance().LogInfo(std::string("User typed: \"") + input_buffer + "\"");
                        input_buffer[0] = '\0';  // Clear the input
                    }
                    reclaim_focus = true;
                }
                
                // Auto-focus on text input
                ImGui::SetItemDefaultFocus();
                if (reclaim_focus)
                    ImGui::SetKeyboardFocusHere(-1);
            }
            ImGui::End();
        }

        //
        // end turn is called by the game code at the end of each turn
        // this is where we check for a winner
        //
        void EndOfTurn() 
        {
        }
}
