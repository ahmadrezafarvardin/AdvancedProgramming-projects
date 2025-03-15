// signup.js
document.addEventListener("DOMContentLoaded", () => {
  const signupForm = document.getElementById("signupForm");
  const messageElement = document.getElementById("message");

  messageElement.classList.add("message");

  signupForm.addEventListener("submit", async (e) => {
    e.preventDefault();

    messageElement.style.display = "none";
    messageElement.textContent = "";
    messageElement.classList.remove("message-success", "message-error");

    const username = document.getElementById("username").value;
    const password = document.getElementById("password").value;
    const confirmPassword = document.getElementById("confirmPassword").value;

    if (password !== confirmPassword) {
      messageElement.textContent = "Passwords do not match";
      messageElement.style.display = "block";
      messageElement.classList.add("message-error");
      return;
    }

    try {
      const response = await API.signup(username, password);

      messageElement.textContent = response;
      messageElement.style.display = "block";
      messageElement.classList.add("message-success");

      if (response === "Signup successful") {
        disableForm(true);
        setTimeout(() => {
          window.location.href = "/pages/login.html";
        }, 2000);
      }
    } catch (error) {
      messageElement.textContent = error;
      messageElement.style.display = "block";
      messageElement.classList.add("message-error");
    }
  });

  function disableForm(disabled) {
    const elements = signupForm.elements;
    for (let i = 0; i < elements.length; i++) {
      elements[i].disabled = disabled;
    }
  }
});
