async function setLamps() {
    let response = await fetch('http://10.9.11.111');
    let data = await response.json();
    console.log(data);

    let fragment = document.createDocumentFragment();
    const gallery = document.querySelector('.gallery');
    for (let i = 0; i < data.pins.length; i++) {
        const lamp = document.createElement('article');
        lamp.innerHTML = `
            <h2>${data.pins[i].name}</h2>
            `
        switch (data.pins[i].type) {
        case "Standard":
            if (data.pins[i].state == 1) {
                lamp.innerHTML += `<p>Esta lámpara está encendida</p>`
                lamp.innerHTML += `<button class="PowerOff lampButton" lamp="${data.pins[i].number}">Apagar</button>`
            } else {
                lamp.innerHTML += `<p>Esta lámpara está apagada</p>`
                lamp.innerHTML += `<button class="PowerOn lampButton" lamp="${data.pins[i].number}">Encender</button>`
            }
            break;
        case "Proximity":
            if (data.pins[i].state == 1) {
                lamp.innerHTML += `<p>Esta lámpara está habilitada</p>`
                lamp.innerHTML += `<button class="PowerDisable lampButton" lamp="${data.pins[i].number}">Deshabilitar</button>`
            } else {
                lamp.innerHTML += `<p>Esta lámpara está deshabilitada</p>`
                lamp.innerHTML += `<button class="PowerEnable lampButton" lamp="${data.pins[i].number}">Habilitar</button>`
            }
            break;
        }
        fragment.appendChild(lamp);
    }
    gallery.appendChild(fragment);

    const lamps = document.querySelectorAll('.lampButton');
    lamps.forEach(lamp => {
        lamp.addEventListener('click', async () => {
            const lampNumber = lamp.getAttribute('lamp');
            let url, response, data;

            if (lamp.classList.contains('PowerOn') || lamp.classList.contains('PowerEnable')) {
                url = `http://10.9.11.111/?P=${lampNumber}1`;
            } else if (lamp.classList.contains('PowerOff') || lamp.classList.contains('PowerDisable')) {
                url = `http://10.9.11.111/?P=${lampNumber}0`;
            } else {
                return;
            }

            try {
                response = await fetch(url, { method: 'POST' });
                if (!response.ok) {
                    throw new Error(`HTTP error! status: ${response.status}`);
                }
                data = await response.json();
                console.log(data);

                switch (data.type) {
                case "Standard":
                    if (data.new_state == 1) {
                        lamp.classList.remove('PowerOn');
                        lamp.classList.add('PowerOff');
                        lamp.innerHTML = "Apagar";
                        lamp.previousElementSibling.innerHTML = "Esta lámpara está encendida";
                    } else {
                        lamp.classList.remove('PowerOff');
                        lamp.classList.add('PowerOn');
                        lamp.innerHTML = "Encender";
                        lamp.previousElementSibling.innerHTML = "Esta lámpara está apagada";
                    }
                    break;
                case "Proximity":
                    if (data.new_state == 1) {
                        lamp.classList.remove('PowerEnable');
                        lamp.classList.add('PowerDisable');
                        lamp.innerHTML = "Deshabilitar";
                        lamp.previousElementSibling.innerHTML = "Esta lámpara está habilitada";
                    } else {
                        lamp.classList.remove('PowerDisable');
                        lamp.classList.add('PowerEnable');
                        lamp.innerHTML = "Habilitar";
                        lamp.previousElementSibling.innerHTML = "Esta lámpara está deshabilitada";
                    }
                    break;
                }
            } catch (error) {
                console.error('Error en la petición:', error);
                alert('Ocurrió un error al intentar cambiar el estado de la lámpara.');
            }
        });
    });

}

setLamps()