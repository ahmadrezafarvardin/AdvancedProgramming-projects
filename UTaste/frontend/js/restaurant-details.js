document.addEventListener("DOMContentLoaded", () => {
  const searchForm = document.getElementById("restaurantSearchForm");
  const detailsContainer = document.getElementById(
    "restaurantDetailsContainer"
  );
  const loadingIndicator = document.getElementById("loadingIndicator");
  const errorContainer = document.getElementById("errorContainer");

  searchForm.addEventListener("submit", async (e) => {
    e.preventDefault();

    detailsContainer.style.display = "none";
    detailsContainer.innerHTML = "";
    errorContainer.style.display = "none";
    errorContainer.textContent = "";
    errorContainer.className = "error-message";

    const restaurantName = document
      .getElementById("restaurantNameInput")
      .value.trim();

    if (!restaurantName) {
      errorContainer.textContent = "Please enter a restaurant name";
      errorContainer.style.display = "block";
      errorContainer.classList.add("message-error");
      return;
    }

    try {
      loadingIndicator.style.display = "block";

      const details = await API.getRestaurantDetails(restaurantName);

      detailsContainer.innerHTML = `<pre class="message-success">${details}</pre>`;
      detailsContainer.style.display = "block";
    } catch (error) {
      errorContainer.textContent = error;
      errorContainer.style.display = "block";
      errorContainer.classList.add("message-error");
    } finally {
      loadingIndicator.style.display = "none";
    }
  });
});
