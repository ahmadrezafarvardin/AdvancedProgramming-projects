// my-reservations.js
document.addEventListener("DOMContentLoaded", () => {
  const viewAllBtn = document.getElementById("viewAllReservations");
  const restaurantSearchForm = document.getElementById("restaurantSearchForm");
  const specificReservationForm = document.getElementById(
    "specificReservationForm"
  );
  const resultsContainer = document.getElementById("reservationsResults");
  const loadingIndicator = document.getElementById("loadingIndicator");
  const noResultsMessage = document.getElementById("noResultsMessage");
  const errorMessage = document.getElementById("errorMessage");

  function clearResults() {
    resultsContainer.innerHTML = "";
    noResultsMessage.style.display = "none";
    errorMessage.style.display = "none";
  }

  async function fetchAndDisplayReservations(
    restaurantName = "",
    reservationId = null
  ) {
    try {
      loadingIndicator.style.display = "block";
      clearResults();

      const response = await API.getReserves(restaurantName, reservationId);
      displayResults(response);
    } catch (error) {
      if (error.includes("Please login first")) {
        window.location.href = "/";
      } else {
        showError(error);
      }
    } finally {
      loadingIndicator.style.display = "none";
    }
  }

  function displayResults(results) {
    if (!results || results.trim() === "") {
      noResultsMessage.style.display = "block";
      return;
    }

    const reservations = results
      .split("\n")
      .filter((line) => line.trim() !== "");

    if (reservations.length === 0) {
      noResultsMessage.style.display = "block";
      return;
    }

    resultsContainer.innerHTML = reservations
      .map(
        (reservation) => `
          <div class="reservation-item">
            <pre>${reservation}</pre>
          </div>
        `
      )
      .join("");
  }

  function showError(message) {
    errorMessage.textContent = message;
    errorMessage.style.display = "block";
    resultsContainer.innerHTML = "";
  }

  viewAllBtn.addEventListener("click", () => {
    clearResults();
    fetchAndDisplayReservations();
  });

  restaurantSearchForm.addEventListener("submit", (e) => {
    e.preventDefault();
    const restaurantName = document
      .getElementById("restaurantName")
      .value.trim();
    if (!restaurantName) {
      showError("Please enter a restaurant name");
      return;
    }
    fetchAndDisplayReservations(restaurantName);
  });

  specificReservationForm.addEventListener("submit", (e) => {
    e.preventDefault();
    const restaurantName = document
      .getElementById("specificRestaurant")
      .value.trim();
    const reserveId = document.getElementById("reservationId").value.trim();

    if (!restaurantName || !reserveId) {
      showError("Please fill in both restaurant name and reservation ID");
      return;
    }
    fetchAndDisplayReservations(restaurantName, reserveId);
  });

  fetchAndDisplayReservations();
});
