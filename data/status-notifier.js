fetch("/status")
  .then((response) =>
    response.text()
  )
  .then((data) => {
    if (data) {
      alert(data);
      if (window.location.href.includes("?")) {
        window.location.href = window.location.href.split("?")[0];
      }
    }
  })
  .catch((error) => {
    console.error(error);
  });