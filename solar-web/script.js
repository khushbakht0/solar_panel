let selectedPanel = 0;
let profiles = JSON.parse(localStorage.getItem('solarProfiles')) || [];

const panelData = [
  { name: "N-type Bifacial", output: 585, price: 18135 },
  { name: "A-Grade", output: 550, price: 13570 },
  { name: "Mono Facial", output: 560, price: 15540 }
];

function selectPanel(el, idx) {
  selectedPanel = idx;
  document.querySelectorAll('.panel').forEach(p => p.classList.remove('selected'));
  el.classList.add('selected');
}

function generateAppliances() {
  const count = parseInt(document.getElementById('appcount').value);
  const div = document.getElementById('appliances');
  div.innerHTML = '';
  if (!count || count <= 0) return;

  const header = document.createElement('div');
  header.className = 'appliance-row';
  header.innerHTML = `<label>Appliance Name</label><label>Quantity</label><label>Watts each</label>`;
  div.appendChild(header);

  for (let i = 0; i < count; i++) {
    const row = document.createElement('div');
    row.className = 'appliance-row';
    row.innerHTML = `
      <input type="text" id="aname${i}" placeholder="e.g. AC" />
      <input type="number" id="aqty${i}" placeholder="1" />
      <input type="number" id="awatt${i}" placeholder="1500" />
    `;
    div.appendChild(row);
  }
}

function getAdditionalCost(energy) {
  if (energy <= 1200) return 60000;
  if (energy <= 3000) return 90000;
  if (energy <= 4000) return 150000;
  if (energy <= 6000) return 240000;
  if (energy <= 9000) return 375000;
  if (energy <= 12000) return 800000;
  return 0;
}

function calculate() {
  const name = document.getElementById('name').value || 'My Home';
  const rooms = parseInt(document.getElementById('rooms').value) || 0;
  const appcount = parseInt(document.getElementById('appcount').value) || 0;
  const coverage = parseInt(document.getElementById('coverage').value);
  const password = Math.floor(10000 + Math.random() * 90000);

  let totalPower = rooms * 200;
  let applianceList = [];

  for (let i = 0; i < appcount; i++) {
    const aname = document.getElementById(`aname${i}`)?.value || `Appliance ${i+1}`;
    const qty = parseInt(document.getElementById(`aqty${i}`)?.value) || 0;
    const watt = parseInt(document.getElementById(`awatt${i}`)?.value) || 0;
    totalPower += qty * watt;
    applianceList.push({ name: aname, qty, watt });
  }

  const energyCovered = totalPower * (coverage / 100);
  const panel = panelData[selectedPanel];
  const numPanels = Math.ceil(energyCovered / panel.output);
  const additionalCost = getAdditionalCost(energyCovered);
  const totalCost = numPanels * panel.price + additionalCost;

  const profile = {
    name, password, rooms, appcount, applianceList,
    coverage, totalPower, energyCovered,
    panel: panel.name, numPanels, totalCost,
    date: new Date().toLocaleDateString()
  };

  const exists = profiles.find(p => p.name === name);
  if (!exists) {
    profiles.push(profile);
    localStorage.setItem('solarProfiles', JSON.stringify(profiles));
  }

  document.getElementById('r-panels').textContent = numPanels;
  document.getElementById('r-energy').textContent = (energyCovered / 1000).toFixed(1) + ' kW';
  document.getElementById('r-cost').textContent = 'Rs. ' + totalCost.toLocaleString();
  document.getElementById('r-password').textContent = '🔑 Profile password: ' + password;
  document.getElementById('r-summary').textContent =
    `Profile: ${name} | Panel: ${panel.name} | Total power: ${totalPower}W | Coverage: ${coverage}%`;

  const panelCost = numPanels * panel.price;
  document.getElementById('r-breakdown').innerHTML = `
    <div class="breakdown-row">
      <span>Panel cost (${numPanels} x Rs.${panel.price.toLocaleString()})</span>
      <span>Rs. ${panelCost.toLocaleString()}</span>
    </div>
    <div class="breakdown-row">
      <span>Installation & inverter cost</span>
      <span>Rs. ${additionalCost.toLocaleString()}</span>
    </div>
    <div class="breakdown-row total">
      <span>Total</span>
      <span>Rs. ${totalCost.toLocaleString()}</span>
    </div>
  `;

  document.getElementById('results').style.display = 'block';
  document.getElementById('results').scrollIntoView({ behavior: 'smooth' });
  renderProfiles();
}

function renderProfiles() {
  const list = document.getElementById('profile-list');
  if (!list) return;
  list.innerHTML = '';
  if (profiles.length === 0) {
    list.innerHTML = '<p style="color:#999; font-size:13px;">No profiles saved yet.</p>';
    return;
  }
  profiles.forEach((p, i) => {
    const card = document.createElement('div');
    card.className = 'profile-card';
    card.innerHTML = `
      <div class="profile-top">
        <span class="profile-name">${p.name}</span>
        <span class="profile-date">${p.date}</span>
      </div>
      <div class="profile-details">
        <span>${p.numPanels} panels</span>
        <span>${p.panel}</span>
        <span>Rs. ${p.totalCost.toLocaleString()}</span>
      </div>
      <button class="delete-btn" onclick="deleteProfile(${i})">Delete</button>
    `;
    list.appendChild(card);
  });
}

function deleteProfile(i) {
  const pwd = prompt('Enter profile password to delete:');
  if (parseInt(pwd) === profiles[i].password) {
    profiles.splice(i, 1);
    localStorage.setItem('solarProfiles', JSON.stringify(profiles));
    renderProfiles();
    alert('Profile deleted successfully!');
  } else {
    alert('Wrong password!');
  }
}

function toggleDarkMode() {
  document.body.classList.toggle('dark');
  const btn = document.getElementById('darkbtn');
  btn.textContent = document.body.classList.contains('dark') ? '☀️ Light Mode' : '🌙 Dark Mode';
}

window.onload = renderProfiles;