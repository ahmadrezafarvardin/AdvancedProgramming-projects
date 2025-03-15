class API {
  static async login(username, password) {
    try {
      const response = await fetch("/api/login", {
        method: "POST",
        credentials: "same-origin",
        headers: {
          "Content-Type": "application/x-www-form-urlencoded",
        },
        body: `username=${encodeURIComponent(
          username
        )}&password=${encodeURIComponent(password)}`,
      });

      const message = await response.text();
      if (!response.ok) {
        throw message;
      }
      return message;
    } catch (error) {
      console.error("Login error:", error);
      throw error;
    }
  }

  static async signup(username, password) {
    try {
      const response = await fetch("/api/signup", {
        method: "POST",
        headers: {
          "Content-Type": "application/x-www-form-urlencoded",
        },
        body: `username=${encodeURIComponent(
          username
        )}&password=${encodeURIComponent(password)}`,
      });

      const message = await response.text();
      if (!response.ok) {
        throw message;
      }
      return message;
    } catch (error) {
      console.error("Signup error:", error);
      throw error;
    }
  }

  static async logout() {
    try {
      const response = await fetch("/api/logout", {
        method: "POST",
        credentials: "same-origin",
      });

      const message = await response.text();
      if (!response.ok) {
        throw message;
      }
      return message;
    } catch (error) {
      console.error("Logout error:", error);
      throw error;
    }
  }

  static async getRestaurantDetails(restaurantName) {
    try {
      const response = await fetch(
        `/api/get/restaurant_detail?restaurant_name=${encodeURIComponent(
          restaurantName
        )}`,
        {
          method: "GET",
          credentials: "same-origin",
        }
      );

      const message = await response.text();
      if (!response.ok) {
        throw message;
      }
      return message;
    } catch (error) {
      console.error("Restaurant details error:", error);
      throw error;
    }
  }

  static async increaseBudget(amount) {
    try {
      const response = await fetch("/api/increase_budget", {
        method: "POST",
        credentials: "same-origin",
        headers: {
          "Content-Type": "application/x-www-form-urlencoded",
        },
        body: `amount=${encodeURIComponent(amount)}`,
      });

      const message = await response.text();
      if (!response.ok) {
        throw message;
      }
      return message;
    } catch (error) {
      console.error("Increase budget error:", error);
      throw error;
    }
  }

  static async makeReservation(reservationDetails) {
    try {
      const response = await fetch("/api/reserve", {
        method: "POST",
        credentials: "same-origin",
        headers: {
          "Content-Type": "application/x-www-form-urlencoded",
        },
        body: Object.keys(reservationDetails)
          .map(
            (key) =>
              `${encodeURIComponent(key)}=${encodeURIComponent(
                reservationDetails[key]
              )}`
          )
          .join("&"),
      });

      const message = await response.text();
      if (!response.ok) {
        throw new Error(message);
      }
      return message;
    } catch (error) {
      console.error("Reservation error:", error);
      throw error;
    }
  }

  static async getReserves(restaurantName = "", reservationId = null) {
    try {
      let url = "/api/reserves";
      const params = new URLSearchParams();

      if (restaurantName) {
        params.append("restaurant_name", restaurantName);
      }

      if (reservationId !== null) {
        params.append("reserve_id", reservationId);
      }

      url += params.toString() ? `?${params.toString()}` : "";

      const response = await fetch(url, {
        method: "GET",
        credentials: "same-origin",
      });

      const message = await response.text();
      if (!response.ok) {
        throw message;
      }
      return message;
    } catch (error) {
      console.error("Reservations error:", error);
      throw error;
    }
  }
}
