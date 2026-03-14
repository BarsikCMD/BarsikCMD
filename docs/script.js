// ─── Copy button ───
function copyCode(btn) {
  const block = btn.closest('.code-block');
  const lines = block.querySelectorAll('.cmd-text');
  const text = Array.from(lines).map(l => l.textContent).join('\n');
  navigator.clipboard.writeText(text).then(() => {
    btn.textContent = 'скопировано!';
    btn.style.color = 'var(--green)';
    setTimeout(() => { btn.textContent = 'копировать'; btn.style.color = ''; }, 1500);
  });
}

// ─── Demo typewriter ───
const demoScript = [
  { type: 'cmd',   text: 'barsik sysinfo',                              delay: 400 },
  { type: 'out',   text: '🐱 BarsikCMD System Info',                    delay: 180, color: 'var(--green)' },
  { type: 'out',   text: '  CPU   : Intel Core i5-12600K  @3.7GHz',     delay: 80,  color: 'var(--cyan)' },
  { type: 'out',   text: '  RAM   : 7.2 GB / 16.0 GB used',             delay: 80,  color: 'var(--cyan)' },
  { type: 'out',   text: '  Disk  : 124 GB / 512 GB free',              delay: 80,  color: 'var(--cyan)' },
  { type: 'out',   text: '  OS    : Ubuntu 24.04 LTS',                  delay: 80,  color: 'var(--cyan)' },
  { type: 'out',   text: '  Uptime: 3 days, 7h 22m',                    delay: 80,  color: 'var(--cyan)' },
  { type: 'empty', delay: 200 },
  { type: 'cmd',   text: 'barsik ls /home',                             delay: 600 },
  { type: 'out',   text: '📁  Documents    📁  Downloads    📁  Projects', delay: 150, color: 'var(--yellow)' },
  { type: 'out',   text: '📁  Music        🐱  .barsikrc    📄  notes.txt', delay: 80, color: 'var(--yellow)' },
  { type: 'empty', delay: 200 },
  { type: 'cmd',   text: 'barsik find notes',                           delay: 500 },
  { type: 'out',   text: '✓ /home/user/notes.txt',                      delay: 200, color: 'var(--green)' },
  { type: 'out',   text: '✓ /home/user/Documents/notes_2024.txt',       delay: 100, color: 'var(--green)' },
  { type: 'empty', delay: 100 },
];

function runDemo() {
  const out = document.getElementById('typewriter-output');
  out.innerHTML = '';
  let accumulated = 0;

  demoScript.forEach(item => {
    accumulated += item.delay;
    const t = accumulated;

    setTimeout(() => {
      if (item.type === 'empty') {
        const el = document.createElement('div');
        el.style.height = '0.4rem';
        out.appendChild(el);
        return;
      }
      if (item.type === 'cmd') {
        const line = document.createElement('div');
        line.className = 't-line';
        line.innerHTML = `<span class="t-prompt">barsik@linux:~$</span> <span class="t-cmd"></span>`;
        out.appendChild(line);
        typeText(line.querySelector('.t-cmd'), item.text, 28, out);
        return;
      }
      if (item.type === 'out') {
        const line = document.createElement('div');
        line.style.paddingLeft = '8px';
        line.style.color = item.color || 'var(--text2)';
        line.style.fontFamily = "'JetBrains Mono', monospace";
        line.style.fontSize = '0.83rem';
        line.textContent = item.text;
        out.appendChild(line);
      }
    }, t);
  });

  accumulated += 300;
  setTimeout(() => {
    const line = document.createElement('div');
    line.className = 't-line';
    line.innerHTML = `<span class="t-prompt">barsik@linux:~$</span> <span class="t-cursor" style="margin-left:8px"></span>`;
    out.appendChild(line);
  }, accumulated);
}

function typeText(el, text, speed, scrollContainer) {
  let i = 0;
  const timer = setInterval(() => {
    if (i >= text.length) { clearInterval(timer); return; }
    el.textContent += text[i++];
    if (scrollContainer) scrollContainer.scrollTop = scrollContainer.scrollHeight;
  }, speed);
}

// ─── Scroll reveal ───
document.addEventListener('DOMContentLoaded', () => {
  const observer = new IntersectionObserver((entries) => {
    entries.forEach(e => {
      if (e.isIntersecting) {
        e.target.style.opacity = '1';
        e.target.style.transform = 'translateY(0)';
      }
    });
  }, { threshold: 0.1 });

  document.querySelectorAll('.feature-card, .install-step, .commands-table tr, .doc-row').forEach(el => {
    el.style.opacity = '0';
    el.style.transform = 'translateY(12px)';
    el.style.transition = 'opacity 0.4s ease, transform 0.4s ease';
    observer.observe(el);
  });
});