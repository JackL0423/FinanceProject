#include "../include/inputReader.h"
#include "../include/ErrorHandler.h"


std::vector<CSVData>CSVDataReader(const std::string& filename)
{
    try
    {
        std::vector<CSVData> data;
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open file: " + filename);
        }

        std::string line;
        if (!std::getline(file, line)) // Check for empty file
        {
            throw std::runtime_error("File is empty: " + filename);
        }

        // Validate header line
        if (line != "Expiration,StockPrice,StrikePrice,CallPrice")
        {
            throw std::runtime_error("Invalid header format in file: " + filename);
        }

        while (std::getline(file, line))
        {
            std::istringstream ss(line);
            std::string token;
            CSVData rowData;

            try
            {
                // Validate and parse each column
                std::getline(ss, token, ',');
                rowData.setExpiration(std::stoi(token));
                std::getline(ss, token, ',');
                rowData.setStockPrice(std::stod(token));
                std::getline(ss, token, ',');
                rowData.setStrikePrice(std::stoi(token));
                std::getline(ss, token, ',');
                rowData.setCallPrice(std::stod(token));

                data.push_back(rowData);
            }
            catch (const std::exception& e)
            {
                // Log warning for invalid row and continue
                ErrorHandler::logError("Skipping invalid row: " + line + " | Error: " + std::string(e.what()));
            }
        }

        file.close();
        return data;
    }
    catch(const std::exception& e)
    {
        ErrorHandler::logError("Error in CSVDataReader: " + std::string(e.what()));
        throw;
    }
}