import fs from 'fs';
import path from 'path';
import child_process from 'child_process'
import { fileURLToPath } from 'url';
import { marked } from 'marked';
import { baseUrl } from "marked-base-url";

const relative_path = (p) => path.join(path.dirname(fileURLToPath(import.meta.url)), p ?? '');

const current_branch = child_process.execSync('git branch --show-current', { encoding: 'utf8' });
const template = fs.readFileSync(relative_path('index.template.html'), 'utf8');
const readme = fs.readFileSync(relative_path('../README.md'), 'utf8');

const renderer = {
	heading({ text, depth }) {
		const anchor = text.toLowerCase()
			.replace(/<\/?code>/g, '')
			.replace(/[!?]+/g, '')
			.trim()
			.replace(/[^\w]+/g, '-')
		return `<h${depth} id="${anchor}">${text}</h${depth}>`;
	},

	link({ href, text }) {
		const target = href.startsWith('#') ? '' : 'target="_blank"';

		return `<a href="${href}" ${target}>${text}</a>`;
	},
};

marked.use(
	{
		gfm: true,
		renderer: renderer,
	},
	baseUrl(`https://github.com/spatian-dev/artix/blob/${current_branch}/`)
);

const compiled = marked.parse(readme).replace(/^<h1.+>.*<\/h1>/, '');

fs.mkdirSync(relative_path('./static'), { recursive: true });
fs.writeFileSync(relative_path('./static/index.html'), template.replace('%%content%%', compiled));
