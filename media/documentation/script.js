function toggle(header) {
  const body = header.nextElementSibling;
  const chevron = header.querySelector('.chevron');
  body.classList.toggle('hidden');
  chevron.classList.toggle('open');
}

function showAbout(e) {
  e.preventDefault();
  document.getElementById('view-docs').classList.add('hidden');
  document.getElementById('view-about').classList.remove('hidden');
  document.querySelectorAll('.sidebar-link').forEach(l => l.classList.remove('active'));
  e.currentTarget.classList.add('active');
}

function showDocs(e, anchor) {
  e.preventDefault();
  document.getElementById('view-about').classList.add('hidden');
  document.getElementById('view-docs').classList.remove('hidden');
  document.querySelectorAll('.sidebar-link').forEach(l => l.classList.remove('active'));
  e.currentTarget.classList.add('active');
  if (anchor) {
    setTimeout(() => {
      const el = document.getElementById(anchor);
      if (el) el.scrollIntoView({ behavior: 'smooth' });
    }, 50);
  }
}