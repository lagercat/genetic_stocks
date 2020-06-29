# Genetic Stocks

This is our extension for the First Year ARM11 group project from Imperial College. The project had 3 components: an ARM emulator, an ARM assembler and an extension. That is, any substansial C program we wanted to build.

Note: The repository doesn't contain the actual commit history (which was around 750 commits long and included the full project) since the code was developed
on the internal Gitlab platform of the College.

## The Extension
Our extension is a Genetic Algorithm that optimises hyper parameters of MLP Neural Networks which predict closing prices (but it could be more than that! 🦠😷). We first have to **train** and find a neural network and then we can **predict**  stock prices of the company we trained for. Our code is designed for the datasets produced by [Yahoo Finance](https://finance.yahoo.com/) which we think is a reliable and open data source. 

## Data formatting
We read the data from the given CSV and then we format it. Our model takes as inputs the data points of the last  `NO_DAYS - 1` days and predicts the next day's closing price. Thus, we batch our data into `NO_ROWS / NO_DAYS`* rows (that is integer division, the remaining rows being discarded), each formatted row containing the following: 
`OPEN_DAY_1`, `HIGH_DAY_1`, `LOW_DAY_1`, `CLOSE_DAY_1` ... `LOW_DAY_(NO_DAYS - 1)`
`CLOSE_DAY_(NO_DAYS - 1)` and the target value `CLOSE_DAY_NO_DAYS`.

\* `NO_ROWS` simply represents the number of rows of the given dataset and `NO_DAYS` is
a macro which can be set in `extension/libdata/dataops.h`. Because of this, if
`NO_ROWS / NO_DAYS` is smaller than 2 for your dataset, that will trigger an assertion error.

## Running the extension
 1. `make` - makes all the needed libraries and produces the  **train** and **predict** executables
 2. `make test` - runs our testsuite for the entire project
 3. `make clean` - cleans all the executables, the aggregated header files and the .a libraries getting the project back to its initial state

We have 2 executables time which run under the following schemas:

`train <input_csv> <no_generations> <population_size> <mutation_chance>` 

`predict <input_csv(optional, defaults to misc_csv/data.csv)> <path_to_model_produced_by_train>`

The population size must be greater than 1 and the mutation chance is a floating point
number between 0 and 1.

Note that train produces a file called `nn.csv` with the "fittest" neural network produced
by the algorithm. Predict takes as input a CSV in the format produced by Yahoo Finance (just like `train`)
and loads the model from `<path_to_model_produced_by_train>`.

The folder **misc_csv** contains the pretrained neural network, a dataset with the Google Stock
and an example of predictions. If you want to run the python script to visualise
the predictions, you *have* to run predict in test mode. That is, without giving
it the parameter to the csv dataset (just to the neural net). The parameter
will default to **misc_csv/data.csv**.

## Authors and acknowledgment
The full project is the result of 3 weeks of (hard) work done by: Alex Usher, Euan Scott-Watson, Sebastian Males and Rob Barr. We **do not** claim authorship
of the test library which was written by [Duncan White](https://www.doc.ic.ac.uk/~dcw/). We hope you will enjoy our effort.

							   
