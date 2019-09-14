#ifndef F9SIM_PHYSICS_IDATASOURCE_HPP_
#define F9SIM_PHYSICS_IDATASOURCE_HPP_

#include <string>

namespace Physics {

    // Subclasses of IDataSource produce simulation data that is to be saved in the output CSV file.
    // Not all simulation data needs to be saved. 
    // Each IDataSource subclass will know how to interpret a specific section of a row in the CSV table, so that combined,
    // they cover the entire row.
    class IDataSource {
    private:
        // The index of the first data point owned by this class, in the complete CSV row
        char mFirstIdx;

        // The number of data points that this class is responsible for in the complete CSV row
        char mNumDataPoints;

    public:
        IDataSource() = default;
        ~IDataSource() = default;

        virtual void outputToCSV(std::string& destRowCSV) const = 0;

    };

}

#endif // F9SIM_PHYSICS_IDATASOURCE_HPP_
