document.addEventListener("DOMContentLoaded", () => {
  const form = document.getElementById("reservationForm");
  const responseMessage = document.getElementById("responseMessage");

  form.addEventListener("submit", async (e) => {
    e.preventDefault();

    responseMessage.style.display = "none";
    responseMessage.innerHTML = "";
    responseMessage.className = "message";

    const restaurantName = document.getElementById("restaurantName").value;
    const tableId = document.getElementById("tableId").value;
    const startTime = document.getElementById("startTime").value;
    const endTime = document.getElementById("endTime").value;
    const foods = document.getElementById("foods").value;

    if (!restaurantName || !tableId || !startTime || !endTime) {
      responseMessage.textContent = "Please fill in all required fields";
      responseMessage.style.display = "block";
      responseMessage.classList.add("message-error");
      return;
    }

    if (
      parseInt(startTime) < 0 ||
      parseInt(startTime) > 23 ||
      parseInt(endTime) < 1 ||
      parseInt(endTime) > 24 ||
      parseInt(startTime) >= parseInt(endTime)
    ) {
      responseMessage.textContent =
        "Invalid time range. Start time must be 0-23, end time 1-24, and start time must be less than end time.";
      responseMessage.style.display = "block";
      responseMessage.classList.add("message-error");
      return;
    }

    const reservationDetails = {
      restaurant_name: restaurantName,
      table_id: tableId,
      start_time: startTime,
      end_time: endTime,
      foods: foods,
    };

    try {
      const result = await API.makeReservation(reservationDetails);

      responseMessage.innerHTML = `<pre>${result}</pre>`;
      responseMessage.style.display = "block";
      responseMessage.classList.add(
        result.toLowerCase().includes("error")
          ? "message-error"
          : "message-success"
      );

      if (!result.toLowerCase().includes("error")) {
        form.reset();
      }
    } catch (error) {
      responseMessage.textContent =
        error.message || "Failed to make reservation";
      responseMessage.style.display = "block";
      responseMessage.classList.add("message-error");
    }
  });
});
