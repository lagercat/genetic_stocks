import pandas as pd
import plotly.graph_objs as go

# Opens the CSV file into columns
df = pd.read_csv('predictions.csv')

# These two lines will draw the two lines of Predictions against Data and 
# Close against date
trace = go.Scatter(x = df['Date'], y=df['Predictions'], name='Predicted price')
trace2 = go.Scatter(x = df['Date'], y=df['Close'], name='Close price')

# This two lines are for setting up the graph title and colours and adding
# the two lines to the graph
layout = go.Layout(title='Share Price over Time', plot_bgcolor='rgb(230, 230, 230)', showlegend=True)
fig = go.Figure(data=[trace, trace2], layout=layout)

# Then finally show the graph running on local host
fig.show()
