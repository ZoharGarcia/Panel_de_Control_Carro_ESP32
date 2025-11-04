const slider = document.getElementById('speedSlider');
const label = document.getElementById('speedLbl');
slider.addEventListener('input', () => {
  label.textContent = slider.value + '%';
});
