document.querySelector(".gpio-4-on").onclick = () => {button_click(4, 1)};
document.querySelector(".gpio-4-off").onclick = () => {button_click(4, 0)};

function button_click(pinNum, pinState){
  fetch(`http://${window.location.hostname}:8080`, {
      method: 'PUT',
      headers: {
        'Content-Type': 'application/json',
      },
      body: `${pinNum} ${pinState}`,
    })  
    .then(response => response.text())
    .then(data => {
      let button = document.querySelector(".gpio-4");
  
      if (data == 1){
        button.style.backgroundColor = "red";
        button.textContent = "on";
      }
      else{
        button.style.backgroundColor = "white";
        button.textContent = "off";
      }
    })
    .catch((error) => {
      console.error('Error:', error);
    });
}


function poll_gpio_state() {
  fetch(`http://${window.location.hostname}:8080`, {
    method: 'STAT',
    body: '17'
  })
  .then(response => response.text())
  .then(data => {
    let button = document.querySelector(".gpio-17-input");

    if (data == 1){
      button.style.backgroundColor = "red";
      button.textContent = "on";
    }
    else{
      button.style.backgroundColor = "white";
      button.textContent = "off";
    }
  })
  .catch((error) => {
    console.error('Error:', error);
  });
}



setInterval(poll_gpio_state, 500);