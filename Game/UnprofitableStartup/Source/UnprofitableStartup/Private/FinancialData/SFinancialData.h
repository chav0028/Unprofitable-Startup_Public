// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 

#pragma once

#include "CoreMinimal.h"
#include "SFinancialData.generated.h" // Generated file must match name of the file, and not precisely the struct



USTRUCT(BlueprintType,
	meta = (ToolTip = "Representation of the financial data of a company"))
struct FSFinancialData
{
	GENERATED_BODY()

public:
	// Financial terms definitions from https://www.investopedia.com/

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (ToolTip = "The date of the financial data"))
	FDateTime m_date;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (ToolTip = "Measure of the amount of cash generated by a company's normal business operations."))
	float m_operatingCashFlow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (ToolTip = "Funds used by a company to acquire, upgrade, and maintain physical assets such as property, plants, buildings, technology, or equipment"))
	float m_capitalExpenditure;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (ToolTip = "The profit a company makes after deducting the costs associated with producing and selling its products or the costs associated with its service"))
	float m_grossProfit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (ToolTip = "The money generated from normal business operations"))
	float m_totalRevenue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (ToolTip = "The total cost of manufacturing and delivering a product or service to consumers"))
	float m_costOfRevenue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (ToolTip = "Expense that a business incurs through its normal business operations"))
	float m_operatingExpenses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (ToolTip = "Value of the shares of the company in the stock market"))
	float m_stockValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (ToolTip = "Company's overall profitability after all expenses and costs have been deducted from total revenue"))
	float m_netIncome;
};
