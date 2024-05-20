// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 

#pragma once

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h" 

// Flays the tests will have
static const uint32_t S_TEST_FLAGS = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter;

// The scaling data JSON file path (relative to Content folder)
static const FString S_SCALING_DATA_JSON_PATH = "Data/scaling_data_information.json";

// The ONNX model path for the Neural Network
static const FString S_ONNX_NN_PATH = "ONNX-Models/netIncome_neural_net";