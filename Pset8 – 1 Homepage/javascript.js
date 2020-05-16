

// This function loads on load

function loadItems() {
    
    const taskContainer = document.getElementById('taskContainer');
    const localStorage = window.localStorage;
    const seuss = document.getElementById('seuss1');

    // Convert keys into an array and then sort it

    var keyList = [];

    for (let y = 0; y < localStorage.length; y++) {
        var k = localStorage.key(y);
        keyList.push(k);
    }

    keyList = keyList.sort();

    // --------------------------------------------

    for (let i = 0; i < localStorage.length; i++) {
        var key = keyList[i];
        var value = JSON.parse(localStorage.getItem(key));

        if (value[1] == 1) {
            taskContainer.innerHTML +=
                '<li class="task strike" onclick="strike(\'' + key + '\')" ondblclick="unstrike(\'' + key + '\')" id="' + key +  '">' +
                value[0] +
                '</li>'; 
        }
        else if (value[1] == 0) {
            taskContainer.innerHTML +=
                '<li class="task" onclick="strike(\'' + key + '\')" ondblclick="unstrike(\'' + key + '\')" id="' + key +  '">' +
                value[0] +
                '</li>'; 
        }

    }

    if (localStorage.length > 0 || localStorage.length === 0) {
        seuss.innerHTML += '<p class="seuss">' + 
            '<span class="blue">Click</span> to mark as done ' +
            '<span class="blue">Double Click</span> to undo' +
            '<span class="hidemobile"> <span class="blue">Shift Click</span> to delete </span>' +
            '</p>';
    }

    if (localStorage.length === 0) {

        taskContainer.innerHTML +=
            '<li class="task">A simple task app</li>' +
            '<li class="task">to help manage your day</li>' +
            '<li class="task">and achieve your goals!</li>';
            
            // '<li class="task grey">of the sky.</li>';
        
        // seuss.innerHTML = '<p class="seuss">A list making app inspired by the french painter <a href="https://www.google.com/search?q=yves+klein&source=lnms&tbm=isch&sa=X&ved=2ahUKEwib_OiRp7noAhULo54KHehHBE0Q_AUoAXoECCQQAw&biw=1680&bih=886" target="_blank">Yves Klein</a> .</p>';

        document.getElementById('reset').className = 'hide';
    }

}

function addTask() {
    const inputs = document.querySelectorAll('input');
    const key =  inputs[0].value;
    var value = [inputs[0].value, 0];
    localStorage.setItem(key, JSON.stringify(value));
    location.reload();
}

function deleteTask(key) {
    localStorage.removeItem(key);
    location.reload();
}



// function strike(key) {    
//     const localStorage = window.localStorage;    
//     var key1 = localStorage.key(key);
//     var value = JSON.parse(localStorage.getItem(key1));
//     var value1 = [key, 1];
//     localStorage.setItem(key, JSON.stringify(value1));
//     document.getElementById(key).className = 'task strike';
// }

function strike(key) {
    const localStorage = window.localStorage;    
    var key1 = localStorage.key(key);
    // var value = JSON.parse(localStorage.getItem(key1));
    var value1 = [key, 1];
    localStorage.setItem(key, JSON.stringify(value1));
    document.getElementById(key).className = 'task strike';
    // location.reload();

    // Delete when shift key is pressed
    document.addEventListener("click", function(e){
        if (e.shiftKey === true) {
            deleteTask(key);
        }
    });  


}

function unstrike(key) {
    const localStorage = window.localStorage;    
    var key1 = localStorage.key(key);
    // var value = JSON.parse(localStorage.getItem(key1));
    var value1 = [key, 0];
    localStorage.setItem(key, JSON.stringify(value1));
    document.getElementById(key).className = 'task';
    // location.reload();

    // Delete when shift key is pressed
    document.addEventListener("click", function(e){
        if (e.shiftKey === true) {
            deleteTask(key);
        }
    });  

}

// Mobile only
document.getElementById('form').addEventListener('keypress', function(e) {
    if (e.key === 'Enter') {
        addTask();
    }
});

// Reset list
function clear() {
    localStorage.clear();
    location.reload();
}

document.getElementById('reset').addEventListener('click', function(e) {
    localStorage.clear();
    location.reload();
});





window.onload = loadItems();






