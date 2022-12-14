
from database import Database
from flask import Flask, Response

from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import io

db = Database()

app = Flask(__name__)

@app.route("/")
def plot_png():
    fig = create_figure()
    output = io.BytesIO()
    FigureCanvas(fig).print_png(output)
    return Response(output.getvalue(), mimetype='image/png')

def create_figure():
    sensor_data = db.get_data()
    fig, ax = plt.subplots(figsize = (10,8))

    x = sensor_data.Datetime
    y = sensor_data.Moisture

    ax.plot(x, y)
    ax.set_yticks(range(40,101, 10))
    plt.title("Avocado plant moisture over time")
    return fig

if __name__ == "__main__":
    app.debug = True 
    app.run(host="0.0.0.0", port="1312")