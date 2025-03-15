// main.js
document.addEventListener("DOMContentLoaded", () => {
  updateAuthState();

  const logoutBtn = document.getElementById("logoutBtn");
  if (logoutBtn) {
    logoutBtn.addEventListener("click", handleLogout);
  }
});

function updateAuthState() {
  const isLoggedIn = document.cookie.includes("sessionId=");
  const authButtons = document.getElementById("authButtons");
  const userDashboard = document.getElementById("userDashboard");

  if (isLoggedIn) {
    if (authButtons) authButtons.style.display = "none";
    if (userDashboard) userDashboard.style.display = "block";
  } else {
    if (authButtons) authButtons.style.display = "block";
    if (userDashboard) userDashboard.style.display = "none";
  }
}

function showMessage(message, isSuccess = true) {
  let messageElement = document.getElementById("logoutMessage");
  if (messageElement) {
    messageElement.remove();
  }

  messageElement = document.createElement("div");
  messageElement.id = "logoutMessage";
  messageElement.className =
    "floating-message " + (isSuccess ? "success" : "error");
  messageElement.textContent = message;
  document.body.appendChild(messageElement);
  messageElement.style.display = "block";

  setTimeout(() => {
    messageElement.style.animation = "fadeOut 0.3s ease-out";
    setTimeout(() => {
      messageElement.remove();
    }, 300);
  }, 3000);
}

async function handleLogout() {
  try {
    const response = await API.logout();
    showMessage(response, true);
    setTimeout(() => {
      window.location.href = "/";
    }, 1000);
  } catch (error) {
    showMessage(error, false);
  }
}
