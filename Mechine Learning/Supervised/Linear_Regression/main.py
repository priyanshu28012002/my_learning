import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.datasets import fetch_california_housing
import time

# Load and prepare data
data = fetch_california_housing()
df = pd.DataFrame(data.data, columns=data.feature_names)
df['Target'] = data.target

# Select single feature for visualization
X = df[['MedInc']].values  # Median income
y = df['Target'].values    # House value

# Normalize data
X = (X - X.mean()) / X.std()
y = (y - y.mean()) / y.std()

def gradient_descent_live_plot(X, y, learning_rate=0.1, epochs=50):
    m = len(y)
    theta = np.zeros(2)
    X_b = np.c_[np.ones((m, 1)), X]

    cost_history = []

    # Setup live plot
    plt.ion()
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))

    for epoch in range(epochs):
        # Predictions and errors
        y_pred = X_b.dot(theta)
        errors = y_pred - y

        # Gradients and update
        gradients = (1/m) * X_b.T.dot(errors)
        theta -= learning_rate * gradients

        # Cost
        cost = (errors ** 2).mean()
        cost_history.append(cost)

        # Clear subplots
        ax1.cla()
        ax2.cla()

        # Plot 1: Regression Line
        ax1.scatter(X, y, alpha=0.3, label='Data')
        x_vals = np.linspace(X.min(), X.max(), 100)
        y_vals = theta[0] + theta[1] * x_vals
        ax1.plot(x_vals, y_vals, 'r-', lw=2, label=f'y = {theta[0]:.2f} + {theta[1]:.2f}x')
        ax1.set_title(f'Regression Line - Epoch {epoch+1}')
        ax1.set_xlabel('Normalized Median Income')
        ax1.set_ylabel('Normalized House Value')
        ax1.legend()
        ax1.grid(True)

        # Plot 2: Cost Convergence
        ax2.plot(range(1, epoch + 2), cost_history, 'b-')
        ax2.set_title('Cost Function Convergence')
        ax2.set_xlabel('Epoch')
        ax2.set_ylabel('Cost (MSE)')
        ax2.grid(True)

        plt.tight_layout()
        plt.pause(0.1)  # Pause for update

    plt.ioff()
    plt.show()

    return theta

# Run live gradient descent
final_theta = gradient_descent_live_plot(X, y, learning_rate=0.1, epochs=50)
print(f"\nFinal parameters: Intercept = {final_theta[0]:.4f}, Slope = {final_theta[1]:.4f}")
